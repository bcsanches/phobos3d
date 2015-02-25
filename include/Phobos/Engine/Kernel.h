#ifndef PH_ENGINE_MAIN_H
#define PH_ENGINE_MAIN_H

#include <Phobos/DisableCopy.h>

#include "Phobos/Engine/Core.h"
#include "Phobos/Engine/EngineAPI.h"
#include "Phobos/Engine/ModuleManager.h"

namespace Phobos
{
	namespace Engine
	{
		class PH_ENGINE_API Kernel
		{
			PH_DISABLE_COPY(Kernel);

			public:
			static Kernel &GetInstance();

				Kernel(int argc, char * const argv[], ModuleManager::InitializerList_t list = {});
				~Kernel();

				void RunMainLoop();
				void StopMainLoop();

				void AddModule(std::unique_ptr<Module> &&module, UInt32_t priority = ModulePriorities::NORMAL);
				void AddModule(Module &module, UInt32_t priority = ModulePriorities::NORMAL);
				void RemoveModule(Module &module);

			private:				
				ModuleManager			m_clModuleManager;
				std::unique_ptr<Core>	m_upCore;
		};
	}
}

#endif
