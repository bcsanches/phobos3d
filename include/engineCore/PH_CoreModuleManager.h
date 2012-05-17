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

#ifndef PH_CORE_MODULE_MANAGER_H
#define PH_CORE_MODULE_MANAGER_H

#include <vector>
#include <set>

#include "PH_CoreModule.h"
#include "PH_EngineCoreAPI.h"

namespace Phobos
{
	PH_DECLARE_NODE_PTR(CoreModuleManager);

	enum CoreModulePriorities_e
	{
		LOWEST_PRIORITY			= 0,
		LOW_PRIORITY			= 0xFF,
		MEDIUM_PRIORITY			= 0xFFF,
		NORMAL_PRIORITY			= 0xFFFF,
		HIGH_PRIORITY			= 0xFFFFF,
		HIGHEST_PRIORITY		= 0xFFFFFFF,
		BOOT_MODULE_PRIORITY	= 0xFFFFFFFF
	};

	enum CoreEvents_e
	{
		CORE_EVENT_PREPARE_TO_BOOT,
		CORE_EVENT_BOOT,
		CORE_EVENT_RENDER_READY,
		CORE_EVENT_FINALIZE
	};

	class PH_ENGINE_CORE_API CoreModuleManager_c: public CoreModule_c
	{
		public:
			static CoreModuleManagerPtr_t Create(const String_c &name);

			~CoreModuleManager_c();

			void AddModule(CoreModulePtr_t module, UInt32_t priority = NORMAL_PRIORITY);
			void AddModuleToDestroyList(CoreModule_c &module);
			void RemoveModule(CoreModule_c &module);

			void OnEvent(CoreEvents_e event);

			void LaunchBootModule(const String_c &cfgName);

			void LogCoreModules();

		protected:
			CoreModuleManager_c(const String_c &name, ChildrenMode_e=PRIVATE_CHILDREN);			

			virtual void OnUpdate();
			virtual void OnFixedUpdate();
			virtual void OnPrepareToBoot();
			virtual void OnBoot();
			virtual void OnFinalize();
			virtual void OnRenderReady();

			void CallCoreModuleProc(CoreModuleProc_t proc);

		private:
			void UpdateDestroyList();
			void DispatchEvents();
			void SortModules();

		protected:
			struct ModuleInfo_s
			{
				UInt32_t u32Priority;
				CoreModulePtr_t ipModule;

				static inline bool IsNull(const ModuleInfo_s &info)
				{
					return !info.ipModule;
				}

				inline ModuleInfo_s(CoreModulePtr_t module, UInt32_t priority):
					u32Priority(priority),
					ipModule(module)
				{
				}

				inline ModuleInfo_s(const ModuleInfo_s &rhs):
					u32Priority(rhs.u32Priority),
					ipModule(rhs.ipModule)
				{
				}

				inline bool operator<(const ModuleInfo_s &rhs) const
				{
					return rhs.u32Priority < u32Priority;
				}

				inline bool operator==(const CoreModule_c &module) const
				{
					return ipModule.get() == &module;
				}

				inline bool operator==(const CoreModulePtr_t module) const
				{
					return ipModule == module;
				}
			};

			typedef std::vector<ModuleInfo_s>	ModulesVector_t;
			ModulesVector_t						vecModules;

			typedef std::set<CoreModulePtr_t>	ModulesSet_t;
			ModulesSet_t						setModulesToDestroy;

			typedef std::vector<CoreEvents_e>	EventsVector_t;
			EventsVector_t						vecEvents;

			bool								fLaunchedBoot;
			bool								fPendingSort;
			bool								fPendingRemoveErase;
	};
}

#endif
