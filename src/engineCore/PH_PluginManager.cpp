/*
Phobos 3d
  September 2010

  Copyright (C) 2005-2010 Bruno Crivelari Sanches

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Bruno Crivelari Sanches bcsanches@gmail.com
*/

#include "PH_PluginManager.h"

#include <PH_ContextUtils.h>
#include <PH_Error.h>
#include <PH_Exception.h>
#include <PH_Kernel.h>

#include "PH_Console.h"
#include "PH_Plugin.h"

namespace Phobos
{
	PluginManagerPtr_t PluginManager_c::ipInstance_gl;

	PluginManagerPtr_t &PluginManager_c::CreateInstance(void)
	{
		PH_ASSERT(!ipInstance_gl);

		ipInstance_gl.reset(new PluginManager_c());

		return ipInstance_gl;
	}

	PluginManagerPtr_t &PluginManager_c::GetInstance(void)
	{
		return ipInstance_gl;
	}

	void PluginManager_c::ReleaseInstance(void)
	{
		ipInstance_gl.reset();
	}

	PluginManager_c::PluginManager_c():
		CoreModule_c("PluginManager", PRIVATE_CHILDREN),
		cmdLoadPlugin("loadPlugin")
	{
		cmdLoadPlugin.SetProc(PH_CONTEXT_CMD_BIND(&PluginManager_c::CmdLoadPlugin, this));
	}

	PluginManager_c::~PluginManager_c()
	{
	}

	void PluginManager_c::OnPrepareToBoot()
	{
		ConsolePtr_t console = Console_c::GetInstance();

		console->AddContextCmd(cmdLoadPlugin);
	}

	void PluginManager_c::LoadPlugin(const String_c &name)
	{
		this->AddPrivateChild(Plugin_c::Create(name));
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
}
