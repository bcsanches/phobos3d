/*
Phobos 3d
October 2010
Copyright (c) 2005-2011 Bruno Sanches  http://code.google.com/p/phobos3d

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#include "PH_CoreModuleManager.h"

#include <algorithm>

#include <boost/foreach.hpp>

#include <PH_Error.h>
#include <PH_Exception.h>
#include <PH_Kernel.h>
#include <PH_Memory.h>

#include "PH_BootModule.h"

namespace Phobos
{	
	CoreModuleManagerPtr_t CoreModuleManager_c::Create(const String_c &name)
	{
		return CoreModuleManagerPtr_t(PH_NEW CoreModuleManager_c(name));
	}

	CoreModuleManager_c::CoreModuleManager_c(const Phobos::String_c &name, UInt32_t flags):
		CoreModule_c(name, flags),
		fLaunchedBoot(false),
		fPendingSort(false),
		fPendingRemoveErase(false)
	{
		//empty
	}

	CoreModuleManager_c::~CoreModuleManager_c()
	{
		//empty
	}

	void CoreModuleManager_c::SortModules()
	{
		if(fPendingSort)
		{
			std::sort(vecModules.begin(), vecModules.end());
			fPendingSort = false;
		}
	}

	void CoreModuleManager_c::OnPrepareToBoot()
	{
		this->OnEvent(CoreEvents::PREPARE_TO_BOOT);
	}

	void CoreModuleManager_c::OnBoot()
	{
		this->OnEvent(CoreEvents::BOOT);
	}

	void CoreModuleManager_c::OnUpdate()
	{
		this->UpdateDestroyList();
		this->DispatchEvents();
		this->SortModules();

		this->CallCoreModuleProc(&CoreModule_c::OnUpdate);	
	}

	void CoreModuleManager_c::OnFixedUpdate()
	{
		this->UpdateDestroyList();
		this->DispatchEvents();		
		this->SortModules();

		this->CallCoreModuleProc(&CoreModule_c::OnFixedUpdate);		
	}

	void CoreModuleManager_c::OnRenderReady()
	{
		this->OnEvent(CoreEvents::RENDER_READY);
		this->OnEvent(CoreEvents::START);
	}

	void CoreModuleManager_c::OnFinalize()
	{				
		this->OnEvent(CoreEvents::FINALIZE);
		this->DispatchEvents();
		this->UpdateDestroyList();			
	}

	void CoreModuleManager_c::CallCoreModuleProc(CoreModuleProc_t proc)
	{
		for(size_t i = 0, len = vecModules.size();i < len; ++i)		
		{	
			if(!vecModules[i].pclModule)
				continue;

			((*vecModules[i].pclModule).*proc)();
		}
	}

	void CoreModuleManager_c::AddModule(CoreModule_c &module, UInt32_t priority)
	{
		if((priority == CoreModulePriorities::BOOT_MODULE) && (dynamic_cast<BootModule_c *>(&module) == NULL))
			PH_RAISE(INVALID_PARAMETER_EXCEPTION, "CoreModuleManager_c::AddModule", "Only BootModule can use BOOT_MODULE_PRIORITY, module name: " + module.GetName());

		this->AddPrivateChild(module);		

		vecModules.push_back(ModuleInfo_s(module, priority));	
		fPendingSort = true;
	}

	void CoreModuleManager_c::AddModuleToDestroyList(CoreModule_c &module)
	{
		ModulesVector_t::iterator it = std::find(vecModules.begin(), vecModules.end(), module);
		if(it == vecModules.end())
		{
			std::stringstream stream;
			stream << "Module " << module.GetName() << " not found on core list.";

			PH_RAISE(OBJECT_NOT_FOUND_EXCEPTION, "CoreModuleManager_c::AddModuleToDestroyList", stream.str());
		}
		
		setModulesToDestroy.insert(it->pclModule);
	}

	void CoreModuleManager_c::RemoveModule(CoreModule_c &module)
	{
		ModulesVector_t::iterator it = std::find(vecModules.begin(), vecModules.end(), module);
		if(it == vecModules.end())
		{
			std::stringstream stream;
			stream << "Module " << module.GetName() << " not found on core list.";

			PH_RAISE(OBJECT_NOT_FOUND_EXCEPTION, "CoreModuleManager_c::AddModuleToDestroyList", stream.str());
		}
		
		
		this->RemoveChild(module);		
		it->pclModule = NULL;
		fPendingRemoveErase = true;
	}	

	void CoreModuleManager_c::UpdateDestroyList()
	{
		if(fPendingRemoveErase)
		{
			fPendingRemoveErase = false;
			vecModules.erase(std::remove_if(vecModules.begin(), vecModules.end(), ModuleInfo_s::IsNull));
		}

		BOOST_FOREACH(CoreModule_c *ptr, setModulesToDestroy)
		{			
			ModulesVector_t::iterator it = std::find(vecModules.begin(), vecModules.end(), *ptr);

			PH_ASSERT_MSG(it != vecModules.end(), "Module on removed list is not registered!!!");
						
			vecModules.erase(it);
			this->RemoveChild(*ptr);
			ptr->OnFinalize();
		}

		setModulesToDestroy.clear();
	}

	void CoreModuleManager_c::OnEvent(CoreEvents::Enum event)
	{
		vecEvents.push_back(event);
	}

	void CoreModuleManager_c::DispatchEvents()
	{
		if(vecEvents.empty())
			return;

		//use another vector, so we do not freeze on recursive events
		EventsVector_t tmp;
		tmp.swap(vecEvents);

		BOOST_FOREACH(CoreEvents::Enum event, tmp)
		{
			switch(event)
			{
			case CoreEvents::PREPARE_TO_BOOT:
					this->CallCoreModuleProc(&CoreModule_c::OnPrepareToBoot);
					break;

				case CoreEvents::BOOT:
					this->CallCoreModuleProc(&CoreModule_c::OnBoot);
					break;

				case CoreEvents::RENDER_READY:
					this->CallCoreModuleProc(&CoreModule_c::OnRenderReady);
					break;

				case CoreEvents::START:
					this->CallCoreModuleProc(&CoreModule_c::OnStart);
					break;

				case CoreEvents::FINALIZE:
					this->CallCoreModuleProc(&CoreModule_c::OnFinalize);
					break;

				default:
					PH_ASSERT_MSG(false, "Invalid value on events");
					break;
			}
		}
	}
	
	void CoreModuleManager_c::LaunchBootModule(const String_c &cfgName)
	{
		if(fLaunchedBoot)
			PH_RAISE(INVALID_OPERATION_EXCEPTION, "Core_c::LaunchBootModule", "Boot module already launched");

		std::auto_ptr<CoreModule_c> ptr(PH_NEW BootModule_c(cfgName, *this));
		this->AddModule(*ptr, CoreModulePriorities::BOOT_MODULE);

		ptr.release();
	}	

	void CoreModuleManager_c::LogCoreModules()
	{
		using namespace std;
		stringstream stream;

		stream << "Core modules start: " << endl;		
		BOOST_FOREACH(ModuleInfo_s &m, vecModules)
		{			
			if(!m.pclModule)
				continue;			

			stream << '\t' << m.pclModule->GetName() << ' ' << m.u32Priority << endl;			

			CoreModuleManager_c *subModule = dynamic_cast<CoreModuleManager_c *>(m.pclModule);
			if(subModule)
			{
				Kernel_c::GetInstance().LogMessage(stream.str());
				subModule->LogCoreModules();

				stream.str("");
			}
		}

		stream << "core modules finished." << endl;
		Kernel_c::GetInstance().LogMessage(stream.str());
	}
}