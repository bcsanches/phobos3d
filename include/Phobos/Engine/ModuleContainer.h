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

#ifndef PH_CORE_MODULE_CONTAINER_H
#define PH_CORE_MODULE_CONTAINER_H

#include <vector>
#include <set>

#include "Phobos/Engine/Module.h"
#include "Phobos/Engine/EngineAPI.h"

namespace Phobos
{
	namespace Engine
	{
		namespace ModulePriorities
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

		PH_DECLARE_NODE_PTR(ModuleContainer);
	
		class PH_ENGINE_API ModuleContainer: public Module
		{
			public:				
				//static ModuleManagerPtr_t Create(const String_t &name);

				ModuleContainer(const String_t &name, UInt32_t flags = NodeFlags::PRIVATE_CHILDREN);
				~ModuleContainer();

				void AddModule(Module &module, UInt32_t priority = ModulePriorities::NORMAL);
				void AddModuleToDestroyList(Module &module);
				void RemoveModule(Module &module);

				void LaunchBootModule(const String_t &cfgName, int argc, char *const argv[]);

				void LogCoreModules();				

			protected:				
				virtual void OnUpdate() override;
				virtual void OnFixedUpdate() override;

				virtual void OnInit() override;
				virtual void OnStart() override;
				virtual void OnStarted() override;

				virtual void OnStop() override;
				virtual void OnFinalize() override;				

				void CallModuleProc(ModuleProc_t proc);			

			private:
				void UpdateDestroyList();
				void SortModules();

			protected:
				struct ModuleInfo_s
				{
					UInt32_t m_u32Priority;
					Module *m_pclModule;

					static inline bool IsNull(const ModuleInfo_s &info)
					{
						return !info.m_pclModule;
					}

					inline ModuleInfo_s(Module &module, UInt32_t priority):
						m_u32Priority(priority),
						m_pclModule(&module)
					{
					}

					inline ModuleInfo_s(const ModuleInfo_s &rhs):
						m_u32Priority(rhs.m_u32Priority),
						m_pclModule(rhs.m_pclModule)
					{
					}

					inline bool operator<(const ModuleInfo_s &rhs) const
					{
						return rhs.m_u32Priority < m_u32Priority;
					}

					inline bool operator==(const Module &module) const
					{
						return m_pclModule == &module;
					}

					inline bool operator==(const ModuleInfo_s &info) const
					{
						return m_pclModule == info.m_pclModule;
					}
				};

				typedef std::vector<ModuleInfo_s>	ModulesVector_t;
				ModulesVector_t						m_vecModules;

				typedef std::set<Module *>			ModulesSet_t;
				ModulesSet_t						m_setModulesToDestroy;

				bool								m_fLaunchedBoot;
				bool								m_fPendingSort;
				bool								m_fPendingRemoveErase;
		};
	}
}

#endif
