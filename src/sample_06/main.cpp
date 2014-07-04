/*
Phobos 3d
April 2012
Copyright (c) 2005-2012 Bruno Sanches  http://code.google.com/p/phobos3d

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

/**

This sample shows how to create a minimal "engine" using phobos components. The engine will be able to only create a windows and answer to input commands.

For any rendering, implement your own render system inside the render class.

The game logic system will be fully working, just add modules as needed (pluginmanager, dictionarymanager, gameengine, etc)

For running the sample:
	* Make sure your IDE is pointing to the "work/sample06" as working directory, so the sample can read the "autoexec.cfg"
	* Configure your IDE to allow the program to find the dlls on bin/

For Visual Studio users, go to the project Property Pages, on the "Debugging" page, sets:
	* Working directory: ..\..\..\work\sample06
	* Environment: PATH=$(PHOBOS_DEPS_HOME)\bin

	* Note that "$(PHOBOS_DEPS_HOME)" is assumed to be pointing to PHOBOS DEPS build (https://github.com/bcsanches/phobos_deps)
*/

#include <sstream>

#include "Console.h"
#include "Render.h"

#include <Phobos/Engine/Client.h>
#include <Phobos/Engine/Session.h>
#include <Phobos/Shell/Variable.h>
#include <Phobos/Shell/Utils.h>
#include <Phobos/Engine/Core.h>
#include <Phobos/Memory.h>
#include <Phobos/ProcVector.h>

#include <Phobos/System/Timer.h>

#define SPEED 10

class GameClient: public Phobos::Engine::Client
{
	public:
		GameClient():
			m_iDirX(SPEED),
			m_iDirY(SPEED)
		{
			m_clSprite.m_iX = 150;
			m_clSprite.m_iY = 350;
		}

		virtual void OnFixedUpdate() override;

		virtual void DispatchCommand(Phobos::Engine::IPlayerCmdPtr_t cmd) override
		{
			//ignore
		}

		virtual Phobos::Engine::EscAction HandleEsc(Phobos::Engine::Gui::Form *&outForm) override
		{
			//We ignore ESC for now
			return Phobos::Engine::EscAction::IGNORE_ESC;
		}

	private:
		Sprite m_clSprite;

		int m_iDirX;
		int m_iDirY;
};

void GameClient::OnFixedUpdate()
{
	auto &render = Render::GetInstance();		

	if (m_clSprite.m_iX >= render.GetWindowWidth() - 10)
	{
		m_iDirX = -SPEED;
	}

	if (m_clSprite.m_iY >= render.GetWindowHeight()- 10)
	{
		m_iDirY = -SPEED;
	}

	if (m_clSprite.m_iX < 0)
	{
		m_iDirX = SPEED;
	}

	if (m_clSprite.m_iY < 0)
	{
		m_iDirY = SPEED;
	}

	m_clSprite.m_iX += m_iDirX;
	m_clSprite.m_iY += m_iDirY;

	render.Draw(m_clSprite);
}

class EngineMain
{
	public:
		EngineMain();
		~EngineMain();

		void MainLoop(void);		

	private:			
		Phobos::ProcVector	m_clSingletons;
		GameClient			m_clClient;
};

EngineMain::EngineMain()
{
	using namespace Phobos;

	auto &console = ::Console::CreateInstance();
	m_clSingletons.AddProc(Engine::Console::ReleaseInstance);

	auto &core = Engine::Core::CreateInstance(console, "autoexec.cfg", 0, nullptr);
	m_clSingletons.AddProc(Engine::Core::ReleaseInstance);	
	
	core.AddModule(console);

	auto &session = Engine::Session::CreateInstance();
	m_clSingletons.AddProc(Engine::Session::ReleaseInstance);
	core.AddModule(session);

	auto &render = Render::CreateInstance(console);
	m_clSingletons.AddProc(Render::ReleaseInstance);
	core.AddModule(render, Engine::ModulePriorities::LOWEST);

	session.SetClient(&m_clClient);

	//
	//The ideal is to store those on a config, perhaps autoexec.cfg, see sample 05
	//

	/*
	Bind ESCAPE key from keyboard (kb) with the command quit

	The quit command is automatically created by the engine core (Core class) and it stops the main loop

	*/
	session.Bind("kb", "ESCAPE", "quit");

	/*
	By default the console starts open, so tell the session that we do not want it now
	*/
	session.CloseConsole();


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
	Phobos::Engine::Core::GetInstance().StartMainLoop();
}

int main(int, char **)
{
	//Phobos::EnableMemoryTracker();	
	{
		EngineMain engine;

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
	//Phobos::DumpMemoryLeaks();

	return 0;
}
