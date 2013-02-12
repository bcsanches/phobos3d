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


#include <Phobos/Error.h>
#include <Phobos/Exception.h>
#include <Phobos/Log.h>
#include <Phobos/Memory.h>

#include "PH_BootModule.h"

namespace Phobos
{	
	CoreModuleManagerPtr_t CoreModuleManager::Create(const String_t &name)
	{
		return CoreModuleManagerPtr_t(PH_NEW CoreModuleManager(name));
	}

	CoreModuleManager::CoreModuleManager(const Phobos::String_t &name, UInt32_t flags):
		CoreModule(name, flags),
		m_fLaunchedBoot(false),
		m_fPendingSort(false),
		m_fPendingRemoveErase(false)
	{
		//empty
	}

	CoreModuleManager::~CoreModuleManager()
	{
		//empty
	}

	void CoreModuleManager::SortModules()
	{
		if(m_fPendingSort)
		{
			std::sort(m_vecModules.begin(), m_vecModules.end());
			m_fPendingSort = false;
		}
	}

	void CoreModuleManager::OnPrepareToBoot()
	{
		this->OnEvent(CoreEvents::PREPARE_TO_BOOT);
	}

	void CoreModuleManager::OnBoot()
	{
		this->OnEvent(CoreEvents::BOOT);
	}

	void CoreModuleManager::OnUpdate()
	{
		this->UpdateDestroyList();
		this->DispatchEvents();
		this->SortModules();

		this->CallCoreModuleProc(&CoreModule::OnUpdate);	
	}

	void CoreModuleManager::OnFixedUpdate()
	{
		this->UpdateDestroyList();
		this->DispatchEvents();		
		this->SortModules();

		this->CallCoreModuleProc(&CoreModule::OnFixedUpdate);		
	}

	void CoreModuleManager::OnRenderReady()
	{
		this->OnEvent(CoreEvents::RENDER_READY);
		this->OnEvent(CoreEvents::START);
	}

	void CoreModuleManager::OnFinalize()
	{				
		this->OnEvent(CoreEvents::FINALIZE);
		this->DispatchEvents();
		this->UpdateDestroyList();			
	}

	void CoreModuleManager::CallCoreModuleProc(CoreModuleProc_t proc)
	{
		for(size_t i = 0, len = m_vecModules.size();i < len; ++i)		
		{	
			if(!m_vecModules[i].m_pclModule)
				continue;

			((*m_vecModules[i].m_pclModule).*proc)();
		}
	}

	void CoreModuleManager::AddModule(CoreModule &module, UInt32_t priority)
	{
		if((priority == CoreModulePriorities::BOOT_MODULE) && (dynamic_cast<BootModule *>(&module) == NULL))
			PH_RAISE(INVALID_PARAMETER_EXCEPTION, "CoreModuleManager::AddModule", "Only BootModule can use BOOT_MODULE_PRIORITY, module name: " + module.GetName());

		this->AddPrivateChild(module);		

		m_vecModules.push_back(ModuleInfo_s(module, priority));	
		m_fPendingSort = true;
	}

	void CoreModuleManager::AddModuleToDestroyList(CoreModule &module)
	{
		ModulesVector_t::iterator it = std::find(m_vecModules.begin(), m_vecModules.end(), module);
		if(it == m_vecModules.end())
		{
			std::stringstream stream;
			stream << "Module " << module.GetName() << " not found on core list.";

			PH_RAISE(OBJECT_NOT_FOUND_EXCEPTION, "CoreModuleManager::AddModuleToDestroyList", stream.str());
		}
		
		m_setModulesToDestroy.insert(it->m_pclModule);
	}

	void CoreModuleManager::RemoveModule(CoreModule &module)
	{
		ModulesVector_t::iterator it = std::find(m_vecModules.begin(), m_vecModules.end(), module);
		if(it == m_vecModules.end())
		{
			std::stringstream stream;
			stream << "Module " << module.GetName() << " not found on core list.";

			PH_RAISE(OBJECT_NOT_FOUND_EXCEPTION, "CoreModuleManager::AddModuleToDestroyList", stream.str());
		}
		
		
		this->RemoveChild(module);		
		it->m_pclModule = NULL;
		m_fPendingRemoveErase = true;
	}	

	void CoreModuleManager::UpdateDestroyList()
	{
		if(m_fPendingRemoveErase)
		{
			m_fPendingRemoveErase = false;
			m_vecModules.erase(std::remove_if(m_vecModules.begin(), m_vecModules.end(), ModuleInfo_s::IsNull));
		}

		for(auto ptr : m_setModulesToDestroy)		
		{			
			ModulesVector_t::iterator it = std::find(m_vecModules.begin(), m_vecModules.end(), *ptr);

			PH_ASSERT_MSG(it != m_vecModules.end(), "Module on removed list is not registered!!!");
						
			m_vecModules.erase(it);
			this->RemoveChild(*ptr);
			ptr->OnFinalize();
		}

		m_setModulesToDestroy.clear();
	}

	void CoreModuleManager::OnEvent(CoreEvents::Enum event)
	{
		m_vecEvents.push_back(event);
	}

	void CoreModuleManager::DispatchEvents()
	{
		if(m_vecEvents.empty())
			return;

		//use another vector, so we do not freeze on recursive events
		EventsVector_t tmp;
		tmp.swap(m_vecEvents);

		for(auto event : tmp)		
		{
			switch(event)
			{
				case CoreEvents::PREPARE_TO_BOOT:
					this->CallCoreModuleProc(&CoreModule::OnPrepareToBoot);
					break;

				case CoreEvents::BOOT:
					this->CallCoreModuleProc(&CoreModule::OnBoot);
					break;

				case CoreEvents::RENDER_READY:
					this->CallCoreModuleProc(&CoreModule::OnRenderReady);
					break;

				case CoreEvents::START:
					this->CallCoreModuleProc(&CoreModule::OnStart);
					break;

				case CoreEvents::FINALIZE:
					this->CallCoreModuleProc(&CoreModule::OnFinalize);
					break;

				default:
					PH_ASSERT_MSG(false, "Invalid value on events");
					break;
			}
		}
	}
	
	void CoreModuleManager::LaunchBootModule(const String_t &cfgName, int argc, char *const argv[])
	{
		if(m_fLaunchedBoot)
			PH_RAISE(INVALID_OPERATION_EXCEPTION, "Core::LaunchBootModule", "Boot module already launched");

		std::unique_ptr<CoreModule> ptr(PH_NEW BootModule(cfgName, argc, argv, *this));
		this->AddModule(*ptr, CoreModulePriorities::BOOT_MODULE);

		ptr.release();
	}	

	void CoreModuleManager::LogCoreModules()
	{
		using namespace std;
		stringstream stream;

		stream << "Core modules start: " << endl;		
		for(auto &m : m_vecModules)		
		{			
			if(!m.m_pclModule)
				continue;			

			stream << '\t' << m.m_pclModule->GetName() << ' ' << m.m_u32Priority << endl;			

			CoreModuleManager *subModule = dynamic_cast<CoreModuleManager *>(m.m_pclModule);
			if(subModule)
			{
				LogMessage(stream.str());				
				subModule->LogCoreModules();

				stream.str("");
			}
		}

		stream << "core modules finished." << endl;
		LogMessage(stream.str());
	}
}
