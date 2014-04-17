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

#include <Phobos/OgreEngine/Gui/Manager.h>

#include <Phobos/OgreEngine/Console.h>
#include <Phobos/Engine/Core.h>
#include <Phobos/Engine/EventManagerModule.h>
#include <Phobos/Game/MapWorld.h>
#include <Phobos/Game/Things/SignalManager.h>
#include <Phobos/Game/Things/ModelRendererManager.h>
#include <Phobos/Game/Things/MoverManager.h>
#include <Phobos/Game/Physics/Manager.h>
#include <Phobos/Engine/PluginManager.h>
#include <Phobos/OgreEngine/Render.h>
#include <Phobos/Engine/Session.h>
#include <Phobos/Game/WorldManager.h>

#include <Phobos/Game/Gui/LevelSelector.h>

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
		auto &core = Engine::Core::CreateInstance("autoexec.cfg", argc, argv);
		m_clSingletons.AddProc(Engine::Core::ReleaseInstance);

		Register::Init();
		m_clSingletons.AddProc(Register::Finalize);		

		Engine::EventManagerModule &eventManager = Engine::EventManagerModule::CreateInstance();
		m_clSingletons.AddProc(Engine::EventManagerModule::ReleaseInstance);
		core.AddModule(eventManager);

		auto &console = OgreEngine::Console::CreateInstance();
		m_clSingletons.AddProc(Engine::Console::ReleaseInstance);
		core.AddModule(console);		

		auto &session = Engine::Session::CreateInstance();
		m_clSingletons.AddProc(Engine::Session::ReleaseInstance);
		core.AddModule(session);

		auto &worldManager = Game::WorldManager::CreateInstance();
		m_clSingletons.AddProc(Game::WorldManager::ReleaseInstance);
		core.AddModule(worldManager);

		auto &mapWorld = Game::MapWorld::CreateInstance();
		m_clSingletons.AddProc(Game::MapWorld::ReleaseInstance);
		core.AddModule(mapWorld);

		auto &levelSelector = Game::Gui::LevelSelector::CreateInstance();
		m_clSingletons.AddProc(Game::Gui::LevelSelector::ReleaseInstance);
		core.AddModule(levelSelector);

		auto &gameEventManager = Game::Things::SignalManager::CreateInstance();
		m_clSingletons.AddProc(Game::Things::SignalManager::ReleaseInstance);
		core.AddModule(gameEventManager);

		Engine::PluginManager &pluginManager = Engine::PluginManager::CreateInstance();
		m_clSingletons.AddProc(Engine::PluginManager::ReleaseInstance);
		core.AddModule(pluginManager, Engine::ModulePriorities::NORMAL-1);		

		auto &moverManager = Game::Things::MoverManager::CreateInstance();
		m_clSingletons.AddProc(Game::Things::MoverManager::ReleaseInstance);
		core.AddModule(moverManager);

		auto &physicsManager = Game::Physics::Manager::CreateInstance();
		m_clSingletons.AddProc(Game::Physics::Manager::ReleaseInstance);
		core.AddModule(physicsManager, Engine::ModulePriorities::LOWEST+3);

		auto &modelRendererManager = Game::Things::ModelRendererManager::CreateInstance();
		m_clSingletons.AddProc(Game::Things::ModelRendererManager::ReleaseInstance);
		core.AddModule(modelRendererManager, Engine::ModulePriorities::LOWEST+2);

		auto &guiManager = OgreEngine::Gui::Manager::CreateInstance();
		m_clSingletons.AddProc(Engine::Gui::Manager::ReleaseInstance);
		core.AddModule(guiManager, Engine::ModulePriorities::LOWEST+1);

		auto &render = OgreEngine::Render::CreateInstance();
		m_clSingletons.AddProc(OgreEngine::Render::ReleaseInstance);
		core.AddModule(render, Engine::ModulePriorities::LOWEST);

		core.RegisterCommands(console);
		Register::RegisterCommands(console);
	}

	EngineMain::~EngineMain()
	{
		m_clSingletons.CallAll();
	}			

	/**

		The engine main loop

	*/
	void EngineMain::MainLoop(void)
	{
		Engine::Core::GetInstance().StartMainLoop();
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
