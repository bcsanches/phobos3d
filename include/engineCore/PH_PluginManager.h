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

#include <PH_ContextCmd.h>

#include "PH_CoreModule.h"

#include "PH_EngineCoreAPI.h"

namespace Phobos
{
	PH_DECLARE_NODE_PTR(PluginManager);

	class PH_ENGINE_CORE_API PluginManager_c: public CoreModule_c
	{
		public:
			static PluginManager_c &CreateInstance(void);
			static PluginManager_c &GetInstance(void);
			static void ReleaseInstance(void);

			~PluginManager_c();

			void LoadPlugin(const String_t &name);
			void UnloadPlugin(const String_t &name);

		protected:
			void OnPrepareToBoot();
			void OnFinalize();
			void OnRenderReady();

			void OnUpdate();

		private:
			PluginManager_c();			

			void CmdLoadPlugin(const StringVector_t &args, Context_c &);
			void CmdUnloadPlugin(const StringVector_t &args, Context_c &);

		private:
			ContextCmd_c	cmdLoadPlugin;
			ContextCmd_c	cmdUnloadPlugin;
			std::list<String_t> lstPluginsToActivate;

			bool				fSystemReady;

			// =====================================================
			// STATIC PRIVATE ATTRIBUTES
			// =====================================================
			static PluginManagerPtr_t ipInstance_gl;

	};
}

#endif