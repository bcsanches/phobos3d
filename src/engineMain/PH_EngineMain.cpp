/*
Phobos 3d
April 2010
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

#include <sstream>

#include <Phobos/ProcVector.h>
#include <Phobos/Register/Manager.h>
#include <Phobos/Shell/Variable.h>
#include <Phobos/Shell/Utils.h>
#include <Phobos/System/Timer.h>

#include <Gui/PH_OgreManager.h>

#include <PH_OgreConsole.h>
#include <PH_Core.h>
#include <PH_EventManagerModule.h>
#include <PH_GameEventManager.h>
#include <PH_ModelRendererManager.h>
#include <PH_MoverManager.h>
#include <PH_PhysicsManager.h>
#include <PH_PluginManager.h>
#include <PH_Render.h>
#include <PH_Session.h>
#include <PH_WorldManager.h>

#include <Gui/PH_LevelSelector.h>

namespace Phobos
{
	class EngineMain
	{
		public:
			EngineMain(int argc, char *const argv[]);
			~EngineMain();

			void MainLoop(void);					

		private:			
			ProcVector	m_clSingletons;
	};

	EngineMain::EngineMain(int argc, char * const argv[])
	{
		Core &core = Core::CreateInstance();
		m_clSingletons.AddProc(Core::ReleaseInstance);

		Register::Init();
		m_clSingletons.AddProc(Register::Finalize);		

		EventManagerModule &eventManager = EventManagerModule::CreateInstance();
		m_clSingletons.AddProc(EventManagerModule::ReleaseInstance);
		core.AddModule(eventManager);

		Console &console = OgreConsole::CreateInstance();
		m_clSingletons.AddProc(Console::ReleaseInstance);
		core.AddModule(console);		

		Session &session = Session::CreateInstance();
		m_clSingletons.AddProc(Session::ReleaseInstance);
		core.AddModule(session);

		WorldManager &worldManager = WorldManager::CreateInstance();
		m_clSingletons.AddProc(WorldManager::ReleaseInstance);
		core.AddModule(worldManager);

		Gui::LevelSelector &levelSelector = Gui::LevelSelector::CreateInstance();
		m_clSingletons.AddProc(Gui::LevelSelector::ReleaseInstance);
		core.AddModule(levelSelector);

		GameEventManager &gameEventManager = GameEventManager::CreateInstance();
		m_clSingletons.AddProc(GameEventManager::ReleaseInstance);
		core.AddModule(gameEventManager);

		PluginManager &pluginManager = PluginManager::CreateInstance();
		m_clSingletons.AddProc(PluginManager::ReleaseInstance);
		core.AddModule(pluginManager, CoreModulePriorities::NORMAL-1);		

		MoverManager &moverManager = MoverManager::CreateInstance();
		m_clSingletons.AddProc(MoverManager::ReleaseInstance);
		core.AddModule(moverManager);

		Physics::Manager &physicsManager = Physics::Manager::CreateInstance();
		m_clSingletons.AddProc(Physics::Manager::ReleaseInstance);
		core.AddModule(physicsManager, CoreModulePriorities::LOWEST+3);

		ModelRendererManager &modelRendererManager = ModelRendererManager::CreateInstance();
		m_clSingletons.AddProc(ModelRendererManager::ReleaseInstance);
		core.AddModule(modelRendererManager, CoreModulePriorities::LOWEST+2);

		Gui::OgreManager &guiManager = Gui::OgreManager::CreateInstance();
		m_clSingletons.AddProc(Gui::Manager::ReleaseInstance);
		core.AddModule(guiManager, CoreModulePriorities::LOWEST+1);

		Render &render = Render::CreateInstance();
		m_clSingletons.AddProc(Render::ReleaseInstance);
		core.AddModule(render, CoreModulePriorities::LOWEST);

		core.RegisterCommands(console);
		Register::RegisterCommands(console);

		core.LaunchBootModule("autoexec.cfg", argc, argv);
	}

	EngineMain::~EngineMain()
	{
		Core::GetInstance().Shutdown();

		m_clSingletons.CallAll();
	}			

	/**

		The engine main loop

	*/
	void EngineMain::MainLoop(void)
	{
		Core::GetInstance().MainLoop();
	}
}

extern "C" int SDL_main(int argc, char *argv[])
{
	#ifdef PH_DEBUG
		Phobos::EnableMemoryTracker();

		//Phobos::BreakMemoryAllocation(405);
	#endif

	{
		Phobos::EngineMain engine(argc, argv);

	#ifndef PH_DEBUG
		try
	#endif
		{
			engine.MainLoop();
		}
	#ifndef PH_DEBUG
		catch(std::exception &e)
		{
			std::stringstream stream;
			stream << "main: Unhandled excetion: ";
			stream << e.what();
			Phobos::LogMessage(stream.str());

			exit(EXIT_FAILURE);
		}
	#endif
	}

#ifdef PH_DEBUG
	//Phobos::DumpMemoryLeaks();
#endif

	return 0;
}
