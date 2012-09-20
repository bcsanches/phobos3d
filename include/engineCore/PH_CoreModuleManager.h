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

	namespace CoreModulePriorities
	{
		enum Enum
		{		
			LOWEST		= 0,
			LOW			= 0xFF,
			MEDIUM		= 0xFFF,
			NORMAL		= 0xFFFF,
			HIGH		= 0xFFFFF,
			HIGHEST		= 0xFFFFFFF,
			BOOT_MODULE	= 0xFFFFFFFF
		};
	}

	namespace CoreEvents
	{
		enum Enum
		{
			PREPARE_TO_BOOT,
			BOOT,
			RENDER_READY,
			FINALIZE
		};
	}

	class PH_ENGINE_CORE_API CoreModuleManager_c: public CoreModule_c
	{
		public:
			static CoreModuleManagerPtr_t Create(const String_c &name);

			~CoreModuleManager_c();

			void AddModule(CoreModule_c &module, UInt32_t priority = CoreModulePriorities::NORMAL);
			void AddModuleToDestroyList(CoreModule_c &module);
			void RemoveModule(CoreModule_c &module);

			void OnEvent(CoreEvents::Enum event);

			void LaunchBootModule(const String_c &cfgName);

			void LogCoreModules();

		protected:
			CoreModuleManager_c(const String_c &name, UInt32_t flags = NodeFlags::PRIVATE_CHILDREN);			

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
				CoreModule_c *pclModule;

				static inline bool IsNull(const ModuleInfo_s &info)
				{
					return !info.pclModule;
				}

				inline ModuleInfo_s(CoreModule_c &module, UInt32_t priority):
					u32Priority(priority),
					pclModule(&module)
				{
				}

				inline ModuleInfo_s(const ModuleInfo_s &rhs):
					u32Priority(rhs.u32Priority),
					pclModule(rhs.pclModule)
				{
				}

				inline bool operator<(const ModuleInfo_s &rhs) const
				{
					return rhs.u32Priority < u32Priority;
				}

				inline bool operator==(const CoreModule_c &module) const
				{
					return pclModule == &module;
				}

				inline bool operator==(const ModuleInfo_s &info) const
				{
					return pclModule == info.pclModule;
				}
			};

			typedef std::vector<ModuleInfo_s>	ModulesVector_t;
			ModulesVector_t						vecModules;

			typedef std::set<CoreModule_c *>	ModulesSet_t;
			ModulesSet_t						setModulesToDestroy;

			typedef std::vector<CoreEvents::Enum>	EventsVector_t;
			EventsVector_t							vecEvents;

			bool								fLaunchedBoot;
			bool								fPendingSort;
			bool								fPendingRemoveErase;
	};
}

#endif
