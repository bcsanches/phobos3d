/*
Phobos 3d
September 2010
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

#include "Phobos/Engine/PluginManager.h"

#include <Phobos/Shell/Utils.h>
#include <Phobos/Error.h>
#include <Phobos/Exception.h>
#include <Phobos/Memory.h>

#include "Phobos/Engine/Console.h"
#include "Phobos/Engine/PluginInstance.h"

Phobos::Engine::PluginManagerPtr_t Phobos::Engine::PluginManager::ipInstance_gl;

Phobos::Engine::PluginManager &Phobos::Engine::PluginManager::CreateInstance(void)
{
	PH_ASSERT(!ipInstance_gl);

	ipInstance_gl.reset(PH_NEW PluginManager());

	return *ipInstance_gl;
}

Phobos::Engine::PluginManager &Phobos::Engine::PluginManager::GetInstance(void)
{
	return *ipInstance_gl;
}

void Phobos::Engine::PluginManager::ReleaseInstance(void)
{
	ipInstance_gl.reset();
}

Phobos::Engine::PluginManager::PluginManager():
	Module("PluginManager", NodeFlags::PRIVATE_CHILDREN),
	m_cmdLoadPlugin("loadPlugin"),
	m_cmdUnloadPlugin("unloadPlugin"),
	m_fSystemReady(false)
{
	m_cmdLoadPlugin.SetProc(PH_CONTEXT_CMD_BIND(&PluginManager::CmdLoadPlugin, this));
	m_cmdUnloadPlugin.SetProc(PH_CONTEXT_CMD_BIND(&PluginManager::CmdUnloadPlugin, this));
}

Phobos::Engine::PluginManager::~PluginManager()
{
	//empty
}

void Phobos::Engine::PluginManager::OnPrepareToBoot()
{
	Console &console = Console::GetInstance();

	console.AddContextCommand(m_cmdLoadPlugin);
	console.AddContextCommand(m_cmdUnloadPlugin);
}

void Phobos::Engine::PluginManager::OnFinalize()
{
	this->RemoveAllChildren();
}

void Phobos::Engine::PluginManager::OnRenderReady()
{
	m_fSystemReady = true;
}

void Phobos::Engine::PluginManager::OnUpdate()
{
	if(!m_fSystemReady)
		return;

	while(!m_lstPluginsToActivate.empty())
	{
		String_t pluginName;
		pluginName.swap(m_lstPluginsToActivate.front());
		m_lstPluginsToActivate.pop_front();

		PluginInstance *plugin = static_cast<PluginInstance *>(this->TryGetChild(pluginName));
		if(plugin)
		{
			plugin->Init();
		}
	}
}

void Phobos::Engine::PluginManager::LoadPlugin(const String_t &name)
{
	{
		std::unique_ptr<PluginInstance> plugin(PH_NEW PluginInstance(name));			

		this->AddPrivateChild(std::move(plugin));
	}

	m_lstPluginsToActivate.push_back(name);
}

void Phobos::Engine::PluginManager::UnloadPlugin(const String_t &name)
{
	this->RemoveChild(this->GetChild(name));
}

void Phobos::Engine::PluginManager::CmdLoadPlugin(const Shell::StringVector_t &args, Shell::Context &)
{
	if(args.size() < 2)
	{
		LogMessage("[PluginManager::CmdLoadPlugin] Insuficient parameters, usage: loadPlugin <pluginName> [pluginName1] [pluginName2] [pluginNamen]");
		return;
	}

	for(int i = 1, len = args.size(); i < len; ++i)
	{
		try
		{
			this->LoadPlugin(args[i]);
		}
		catch(Exception &e)
		{
			LogMessage(e.what());
		}
	}
}

void Phobos::Engine::PluginManager::CmdUnloadPlugin(const Shell::StringVector_t &args, Shell::Context &)
{
	if(args.size() < 2)
	{
		LogMessage("[PluginManager::CmdUnloadPlugin] Insuficient parameters, usage: unloadPlugin <pluginName> [pluginName1] [pluginName2] [pluginNamen]");
		return;
	}

	for(int i = 1, len = args.size(); i < len; ++i)
	{
		try
		{
			this->UnloadPlugin(args[i]);
		}
		catch(Exception &e)
		{
			LogMessage(e.what());
		}
	}
}

