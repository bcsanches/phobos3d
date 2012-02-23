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

#include <PH_Console.h>
#include <PH_ContextVar.h>
#include <PH_ContextUtils.h>
#include <PH_Core.h>
#include <PH_DictionaryManager.h>
#include <PH_EventManagerModule.h>
#include <PH_GameEventManager.h>
#include <PH_Kernel.h>
#include <PH_ModelRendererManager.h>
#include <PH_MoverManager.h>
#include <PH_PhysicsManager.h>
#include <PH_PluginManager.h>
#include <PH_ProcVector.h>
#include <PH_Render.h>
#include <PH_Timer.h>
#include <PH_WorldManager.h>

namespace Phobos
{
	class EngineMain_c
	{
		public:
			EngineMain_c();
			~EngineMain_c();

			void MainLoop(void);					

		private:			
			ProcVector_c	clSingletons;
	};

	EngineMain_c::EngineMain_c()
	{
		Kernel_c::CreateInstance("phobos.log");
		CorePtr_t core = Core_c::CreateInstance();
		clSingletons.AddProc(Core_c::ReleaseInstance);

		DictionaryManagerPtr_t dictionaryManager = DictionaryManager_c::CreateInstance();
		clSingletons.AddProc(DictionaryManager_c::ReleaseInstance);

		EventManagerModulePtr_t eventManager = EventManagerModule_c::CreateInstance();
		clSingletons.AddProc(EventManagerModule_c::ReleaseInstance);
		core->AddModule(eventManager);

		ConsolePtr_t console = Console_c::CreateInstance();
		clSingletons.AddProc(Console_c::ReleaseInstance);
		core->AddModule(console);		

		WorldManagerPtr_t worldManager = WorldManager_c::CreateInstance();
		clSingletons.AddProc(WorldManager_c::ReleaseInstance);
		core->AddModule(worldManager);

		GameEventManagerPtr_t gameEventManager = GameEventManager_c::CreateInstance();
		clSingletons.AddProc(GameEventManager_c::ReleaseInstance);
		core->AddModule(gameEventManager);

		PluginManagerPtr_t pluginManager = PluginManager_c::CreateInstance();
		clSingletons.AddProc(PluginManager_c::ReleaseInstance);
		core->AddModule(pluginManager, NORMAL_PRIORITY-1);		

		MoverManagerPtr_t moverManager = MoverManager_c::CreateInstance();
		clSingletons.AddProc(MoverManager_c::ReleaseInstance);
		core->AddModule(moverManager);

		Physics::PhysicsManagerPtr_t physicsManager = Physics::PhysicsManager_c::CreateInstance();
		clSingletons.AddProc(Physics::PhysicsManager_c::ReleaseInstance);
		core->AddModule(physicsManager, LOWEST_PRIORITY+2);

		ModelRendererManagerPtr_t modelRendererManager = ModelRendererManager_c::CreateInstance();
		clSingletons.AddProc(ModelRendererManager_c::ReleaseInstance);
		core->AddModule(modelRendererManager, LOWEST_PRIORITY+1);

		RenderPtr_t render = Render_c::CreateInstance();
		clSingletons.AddProc(Render_c::ReleaseInstance);
		core->AddModule(render, LOWEST_PRIORITY);

		core->RegisterCommands(*console);
		dictionaryManager->RegisterCommands(*console);

		core->LaunchBootModule("autoexec.cfg");
	}

	EngineMain_c::~EngineMain_c()
	{
		Core_c::GetInstance()->Shutdown();

		clSingletons.CallAll();

		Kernel_c::ReleaseInstance();
	}			

	/**

		The engine main loop

	*/
	void EngineMain_c::MainLoop(void)
	{
		Core_c::GetInstance()->MainLoop();
	}
}

int main(int, char **)
{
	Phobos::EngineMain_c engine;

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
		Phobos::Kernel_c::GetInstance().LogMessage(stream.str());

		exit(EXIT_FAILURE);
	}
#endif

	return 0;
}
