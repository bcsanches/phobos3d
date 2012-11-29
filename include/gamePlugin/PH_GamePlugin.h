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

#ifndef PH_GAME_PLUGIN_H
#define PH_GAME_PLUGIN_H

#include "PH_GamePluginAPI.h"

#include <PH_CoreModuleManager.h>
#include <PH_Plugin.h>

namespace Phobos
{
	class GamePlugin_c: public IPluginInstance_c
	{
		public:
			typedef CoreModule_c &(*CreateInstanceProc_t)();
			typedef void (*ReleaseInstanceProc_t)();		

			
			struct Register_s
			{	
				Register_s(CreateInstanceProc_t createProc, ReleaseInstanceProc_t releaseProc);
				Register_s(const Register_s &rhs);

				CreateInstanceProc_t pfnCreate;
				ReleaseInstanceProc_t pfnRelease;
			};

		public:
			void Init();
			void Finalize();

			void AddModule(const Register_s &module);

			static GamePlugin_c &GetInstance();

			static void Configure(const char *moduleName, const char *cfgName);
		
		private:			
			CoreModuleManagerPtr_t ipManager;

			std::vector<Register_s> vecModules;

			static const char *szCfgName_g;
			static const char *szModuleName_g;
	};
}

#define PH_GAME_PLUGIN_ENTRY_POINT(MODULE_NAME, CFG_NAME)	\
	PH_GAME_PLUGIN_ENTRY_POINT_PROC Phobos::IPluginInstance_c *PH_PluginEntryPoint(void)\
	{																					\
		Phobos::GamePlugin_c::Configure(MODULE_NAME, CFG_NAME);							\
																						\
		return &Phobos::GamePlugin_c::GetInstance();									\
	}

#define PH_GAME_PLUGIN_REGISTER_MODULE(X)\
	static Phobos::GamePlugin_c::Register_s X##Register_gl(X##_c::CreateModule, X##_c::ReleaseInstance);

#define PH_GAME_PLUGIN_CREATE_MODULE_PROC_DECL static Phobos::CoreModule_c &CreateModule();
#define PH_GAME_PLUGIN_CREATE_MODULE_PROC_IMPL(X)	\
	Phobos::CoreModule_c &X##_c::CreateModule()		\
	{												\
		return CreateInstance();					\
	}

#endif
