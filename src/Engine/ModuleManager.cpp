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

#include "Phobos/Engine/ModuleManager.h"

#include <algorithm>

#include <Phobos/Error.h>
#include <Phobos/Exception.h>
#include <Phobos/Log.h>
#include <Phobos/Memory.h>

#include "Phobos/Engine/BootModule.h"

Phobos::Engine::ModuleManagerPtr_t Phobos::Engine::ModuleManager::Create(const String_t &name)
{
	return ModuleManagerPtr_t(PH_NEW ModuleManager(name));
}

Phobos::Engine::ModuleManager::ModuleManager(const Phobos::String_t &name, UInt32_t flags):
	Module(name, flags),
	m_fLaunchedBoot(false),
	m_fPendingSort(false),
	m_fPendingRemoveErase(false)
{
	//empty
}

Phobos::Engine::ModuleManager::~ModuleManager()
{
	//empty
}

void Phobos::Engine::ModuleManager::SortModules()
{
	if(m_fPendingSort)
	{
		std::sort(m_vecModules.begin(), m_vecModules.end());
		m_fPendingSort = false;
	}
}

void Phobos::Engine::ModuleManager::OnInit()
{
	this->CallModuleProc(&Module::Init);
}

void Phobos::Engine::ModuleManager::OnStart()
{
	this->CallModuleProc(&Module::Start);
}

void Phobos::Engine::ModuleManager::OnStarted()
{
	this->CallModuleProc(&Module::Started);
}

void Phobos::Engine::ModuleManager::OnUpdate()
{
	this->UpdateDestroyList();
	this->SortModules();

	this->CallModuleProc(&Module::Update);	
}

void Phobos::Engine::ModuleManager::OnFixedUpdate()
{
	this->UpdateDestroyList();
	this->SortModules();

	this->CallModuleProc(&Module::FixedUpdate);
}

void Phobos::Engine::ModuleManager::OnStop()
{
	this->CallModuleProc(&Module::Stop);

	this->UpdateDestroyList();
}

void Phobos::Engine::ModuleManager::OnFinalize()
{				
	this->CallModuleProc(&Module::Finalize);
	
	this->UpdateDestroyList();			
}

void Phobos::Engine::ModuleManager::CallModuleProc(ModuleProc_t proc)
{
	for(size_t i = 0, len = m_vecModules.size();i < len; ++i)		
	{	
		if(!m_vecModules[i].m_pclModule)
			continue;

		((*m_vecModules[i].m_pclModule).*proc)();
	}
}

void Phobos::Engine::ModuleManager::AddModule(Module &module, UInt32_t priority)
{
	if((priority == ModulePriorities::BOOT_MODULE) && (dynamic_cast<BootModule *>(&module) == NULL))
		PH_RAISE(INVALID_PARAMETER_EXCEPTION, "ModuleManager::AddModule", "Only BootModule can use BOOT_MODULE_PRIORITY, module name: " + module.GetName());

	this->AddPrivateChild(module);		

	m_vecModules.push_back(ModuleInfo_s(module, priority));	
	m_fPendingSort = true;
}

void Phobos::Engine::ModuleManager::AddModuleToDestroyList(Module &module)
{
	ModulesVector_t::iterator it = std::find(m_vecModules.begin(), m_vecModules.end(), module);
	if(it == m_vecModules.end())
	{
		std::stringstream stream;
		stream << "Module " << module.GetName() << " not found on core list.";

		PH_RAISE(OBJECT_NOT_FOUND_EXCEPTION, "ModuleManager::AddModuleToDestroyList", stream.str());
	}
		
	m_setModulesToDestroy.insert(it->m_pclModule);
}

void Phobos::Engine::ModuleManager::RemoveModule(Module &module)
{
	ModulesVector_t::iterator it = std::find(m_vecModules.begin(), m_vecModules.end(), module);
	if(it == m_vecModules.end())
	{
		std::stringstream stream;
		stream << "Module " << module.GetName() << " not found on core list.";

		PH_RAISE(OBJECT_NOT_FOUND_EXCEPTION, "ModuleManager::AddModuleToDestroyList", stream.str());
	}
		
		
	this->RemoveChild(module);		
	it->m_pclModule = NULL;
	m_fPendingRemoveErase = true;
}	

void Phobos::Engine::ModuleManager::UpdateDestroyList()
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
		ptr->Finalize();
	}

	m_setModulesToDestroy.clear();
}
	
void Phobos::Engine::ModuleManager::LaunchBootModule(const String_t &cfgName, int argc, char *const argv[])
{
	if(m_fLaunchedBoot)
		PH_RAISE(INVALID_OPERATION_EXCEPTION, "Core::LaunchBootModule", "Boot module already launched");

	std::unique_ptr<Module> ptr(PH_NEW BootModule(cfgName, argc, argv, *this));
	this->AddModule(*ptr, ModulePriorities::BOOT_MODULE);

	ptr.release();
}

void Phobos::Engine::ModuleManager::LogCoreModules()
{
	using namespace std;
	stringstream stream;

	stream << "Core modules start: " << endl;		
	for(auto &m : m_vecModules)		
	{			
		if(!m.m_pclModule)
			continue;			

		stream << '\t' << m.m_pclModule->GetName() << ' ' << m.m_u32Priority << endl;			

		ModuleManager *subModule = dynamic_cast<ModuleManager *>(m.m_pclModule);
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

