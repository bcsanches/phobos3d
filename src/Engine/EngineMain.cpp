#include "Phobos/Engine/EngineMain.h"

#include <Phobos/Memory.h>
#include <Phobos/Register/Manager.h>

#include "Phobos/Engine/Console.h"
#include "Phobos/Engine/Core.h"

namespace Phobos
{
	namespace Engine
	{
		EngineMain::EngineMain(int argc, char *const argv[], ModuleManager::InitializerList_t list):
			m_clModuleManager(list)
		{						
			Register::Load("modules.def");

			auto consoleInfo = m_clModuleManager.CreateModuleEx<Console>("Console");

			auto &core = Engine::Core::CreateInstance(*consoleInfo.m_upModule, "autoexec.cfg", argc, argv);
			m_clSingletons.AddProc(Engine::Core::ReleaseInstance);

			Register::RegisterCommands(*consoleInfo.m_upModule);

			core.AddModule(std::move(consoleInfo.m_upModule), consoleInfo.m_uPriority);

			auto modules = m_clModuleManager.CreateAllModules({ "Console" });

			for (auto &moduleInfo : modules)
			{
				core.AddModule(std::move(moduleInfo.m_upModule), moduleInfo.m_uPriority);
			}
		}

		EngineMain::~EngineMain()
		{
			m_clSingletons.CallAll();
		}

		void EngineMain::StartMainLoop()
		{
			Core::GetInstance().StartMainLoop();
		}
	}
}
