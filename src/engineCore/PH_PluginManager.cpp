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

#include "PH_PluginManager.h"

#include <PH_ContextUtils.h>
#include <PH_Error.h>
#include <PH_Exception.h>
#include <PH_Kernel.h>
#include <PH_Memory.h>

#include "PH_Console.h"
#include "PH_Plugin.h"

namespace Phobos
{
	PluginManagerPtr_t PluginManager_c::ipInstance_gl;

	PluginManager_c &PluginManager_c::CreateInstance(void)
	{
		PH_ASSERT(!ipInstance_gl);

		ipInstance_gl.reset(PH_NEW PluginManager_c());

		return *ipInstance_gl;
	}

	PluginManager_c &PluginManager_c::GetInstance(void)
	{
		return *ipInstance_gl;
	}

	void PluginManager_c::ReleaseInstance(void)
	{
		ipInstance_gl.reset();
	}

	PluginManager_c::PluginManager_c():
		CoreModule_c("PluginManager", NodeFlags::PRIVATE_CHILDREN),
		cmdLoadPlugin("loadPlugin"),
		cmdUnloadPlugin("unloadPlugin"),
		fSystemReady(false)
	{
		cmdLoadPlugin.SetProc(PH_CONTEXT_CMD_BIND(&PluginManager_c::CmdLoadPlugin, this));
		cmdUnloadPlugin.SetProc(PH_CONTEXT_CMD_BIND(&PluginManager_c::CmdUnloadPlugin, this));
	}

	PluginManager_c::~PluginManager_c()
	{
	}

	void PluginManager_c::OnPrepareToBoot()
	{
		Console_c &console = Console_c::GetInstance();

		console.AddContextCmd(cmdLoadPlugin);
		console.AddContextCmd(cmdUnloadPlugin);
	}

	void PluginManager_c::OnFinalize()
	{
		this->RemoveAllChildren();
	}

	void PluginManager_c::OnRenderReady()
	{
		fSystemReady = true;
	}

	void PluginManager_c::OnUpdate()
	{
		if(!fSystemReady)
			return;

		while(!lstPluginsToActivate.empty())
		{
			String_c pluginName;
			pluginName.swap(lstPluginsToActivate.front());
			lstPluginsToActivate.pop_front();

			Plugin_c *plugin = static_cast<Plugin_c *>(this->TryGetChild(pluginName));
			if(plugin)
			{
				plugin->Init();
			}
		}
	}

	void PluginManager_c::LoadPlugin(const String_c &name)
	{
		{
			std::unique_ptr<Plugin_c> plugin(PH_NEW Plugin_c(name));			

			this->AddPrivateChild(std::move(plugin));
		}

		lstPluginsToActivate.push_back(name);
	}

	void PluginManager_c::UnloadPlugin(const String_c &name)
	{
		this->RemoveChild(this->GetChild(name));
	}

	void PluginManager_c::CmdLoadPlugin(const StringVector_t &args, Context_c &)
	{
		if(args.size() < 2)
		{
			Kernel_c::GetInstance().LogMessage("[PluginManager_c::CmdLoadPlugin] Insuficient parameters, usage: loadPlugin <pluginName> [pluginName1] [pluginName2] [pluginNamen]");
			return;
		}

		for(int i = 1, len = args.size(); i < len; ++i)
		{
			try
			{
				this->LoadPlugin(args[i]);
			}
			catch(Exception_c &e)
			{
				Kernel_c::GetInstance().LogMessage(e.what());
			}
		}
	}

	void PluginManager_c::CmdUnloadPlugin(const StringVector_t &args, Context_c &)
	{
		if(args.size() < 2)
		{
			Kernel_c::GetInstance().LogMessage("[PluginManager_c::CmdUnloadPlugin] Insuficient parameters, usage: unloadPlugin <pluginName> [pluginName1] [pluginName2] [pluginNamen]");
			return;
		}

		for(int i = 1, len = args.size(); i < len; ++i)
		{
			try
			{
				this->UnloadPlugin(args[i]);
			}
			catch(Exception_c &e)
			{
				Kernel_c::GetInstance().LogMessage(e.what());
			}
		}
	}
}
