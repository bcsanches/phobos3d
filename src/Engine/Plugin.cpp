/*
Phobos 3d
February 2010
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

#include "Phobos/Engine/Plugin.h"

#include <Phobos/Engine/Core.h>
#include <Phobos/Engine/ModuleManager.h>

#include <Phobos/ProcVector.h>

const char *Phobos::Engine::Plugin::szCfgName_g = NULL;
const char *Phobos::Engine::Plugin::szModuleName_g = NULL;

Phobos::Engine::Plugin &Phobos::Engine::Plugin::GetInstance()
{
	static Plugin plugin;

	return plugin;
}

void Phobos::Engine::Plugin::Configure(const char *moduleName, const char *cfgName)
{
	szModuleName_g = moduleName;
	szCfgName_g = cfgName;
}

void Phobos::Engine::Plugin::Init()
{
	ipManager = Engine::ModuleManager::Create(szModuleName_g);

	auto &core = Engine::Core::GetInstance();
	core.AddModule(*ipManager);	

	for(auto &info : m_vecModules)		
	{
		ipManager->AddModule(info.m_pfnCreate());
	}

	ipManager->LaunchBootModule(szCfgName_g, 0, nullptr);	

	ipManager->Init();
	ipManager->Start();
	ipManager->Started();
}

void Phobos::Engine::Plugin::Finalize()
{
	Engine::Core::GetInstance().RemoveModule(*ipManager);		

	for(auto &info : m_vecModules)		
	{
		info.m_pfnRelease();
	}

	ipManager.reset();
}

void Phobos::Engine::Plugin::AddModule(const Register_s &module)
{
	m_vecModules.push_back(module);
}

Phobos::Engine::Plugin::Register_s::Register_s(CreateInstanceProc_t createProc, ReleaseInstanceProc_t releaseProc):
	m_pfnCreate(createProc),
	m_pfnRelease(releaseProc)
{
	Plugin::GetInstance().AddModule(*this);
}

Phobos::Engine::Plugin::Register_s::Register_s(const Register_s &rhs):
	m_pfnCreate(rhs.m_pfnCreate),
	m_pfnRelease(rhs.m_pfnRelease)
{
	//empty
}

