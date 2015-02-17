#ifndef PH_ENGINE_MAIN_H
#define PH_ENGINE_MAIN_H

#include <Phobos/DisableCopy.h>
#include <Phobos/ProcVector.h>

#include "Phobos/Engine/EngineAPI.h"
#include "Phobos/Engine/ModuleManager.h"

namespace Phobos
{
	namespace Engine
	{
		class PH_ENGINE_API  EngineMain
		{
			PH_DISABLE_COPY(EngineMain);

			public:
				EngineMain(int argc, char * const argv[], ModuleManager::InitializerList_t list = {});
				~EngineMain();

				void StartMainLoop();

			private:
				ProcVector		m_clSingletons;
				ModuleManager	m_clModuleManager;
		};
	}
}

#endif
