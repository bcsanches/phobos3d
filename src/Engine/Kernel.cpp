#include "Phobos/Engine/Kernel.h"

#include <Phobos/Memory.h>
#include <Phobos/Register/Manager.h>

#include "Phobos/Engine/Console.h"
#include "Phobos/Engine/Core.h"

namespace Phobos
{
	namespace Engine
	{
		static Kernel *g_pclInstance = nullptr;

		Kernel &Kernel::GetInstance()
		{
			PH_ASSERT_VALID(g_pclInstance);

			return *g_pclInstance;
		}

		Kernel::Kernel(int argc, char *const argv[], ModuleManager::InitializerList_t list):
			m_clModuleManager(list)
		{	
			PH_ASSERT(!g_pclInstance);

			g_pclInstance = this;

			Register::Load("modules.def");

			auto consoleInfo = m_clModuleManager.CreateModuleEx<Console>("Console");

			m_upCore.reset(PH_NEW Core(*consoleInfo.m_upModule, "autoexec.cfg", argc, argv));		

			Register::RegisterCommands(*consoleInfo.m_upModule);

			m_upCore->AddModule(std::move(consoleInfo.m_upModule), consoleInfo.m_uPriority);

			auto modules = m_clModuleManager.CreateAllModules({ "Console" });

			for (auto &moduleInfo : modules)
			{
				m_upCore->AddModule(std::move(moduleInfo.m_upModule), moduleInfo.m_uPriority);
			}

			m_upCore->InitModules();
		}

		Kernel::~Kernel()
		{
			m_upCore->FinalizeModules();

			PH_ASSERT(this == g_pclInstance);

			g_pclInstance = nullptr;
		}

		void Kernel::AddModule(std::unique_ptr<Module> &&module, UInt32_t priority)
		{
			m_upCore->AddModule(std::move(module), priority);
		}

		void Kernel::AddModule(Module &module, UInt32_t priority)
		{
			m_upCore->AddModule(module, priority);
		}

		void Kernel::RemoveModule(Module &module)
		{
			m_upCore->RemoveModule(module);
		}

		void Kernel::RunMainLoop()
		{
			m_upCore->RunMainLoop();
		}

		void Kernel::StopMainLoop()
		{
			m_upCore->StopMainLoop();
		}
	}
}
