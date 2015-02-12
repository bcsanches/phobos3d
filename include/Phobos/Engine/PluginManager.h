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

#ifndef PH_PLUGIN_MANAGER_H
#define PH_PLUGIN_MANAGER_H

#include <list>


#include <Phobos/Shell/Command.h>

#include "Phobos/Engine/Module.h"

#include "Phobos/Engine/EngineAPI.h"

namespace Phobos
{
	namespace Engine
	{
		class Console;		

		class PH_ENGINE_API PluginManager: public Module
		{
			public:
				static std::unique_ptr<Module> CreateInstance(const String_t &name);
				static PluginManager &GetInstance(void);							

				void LoadPlugin(const String_t &name);
				void UnloadPlugin(const String_t &name);

				void QueuePluginLoad(const String_t &name);

				~PluginManager();
			protected:
				void OnFinalize() override;
				void OnStarted() override;				

			private:				
				PluginManager(const String_t &name);				

				void CmdLoadPlugin(const Shell::StringVector_t &args, Shell::Context &);
				void CmdQueuePluginLoad(const Shell::StringVector_t &args, Shell::Context &);
				void CmdUnloadPlugin(const Shell::StringVector_t &args, Shell::Context &);

			private:
				Shell::Command	m_cmdLoadPlugin;
				Shell::Command	m_cmdQueuePluginLoad;
				Shell::Command	m_cmdUnloadPlugin;

				std::list<String_t> m_lstPluginsToLoad;
				std::list<String_t> m_lstPluginsToActivate;

				bool				m_fSystemReady;				
		};
	}
}

#endif