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
#include <Phobos/Engine/Clocks.h>
#include <Phobos/Engine/Core.h>
#include <Phobos/Memory.h>
#include <Phobos/ProcVector.h>
#include <Phobos/System/AxisButton.h>

#define SPEED 100

class GameObject;

class Controller
{
	public:
		virtual void FixedUpdate(const GameObject &owner) = 0;

		virtual Phobos::Float_t GetDesiredDixX() = 0;
		virtual Phobos::Float_t GetDesiredDixY() = 0;
};

class PlayerController : public Controller
{
	public:
		PlayerController(Phobos::Shell::IContext &context) :
			m_btnUpDown("+down", "=down", "-down", "+up", "=up", "-up"),
			m_btnLeftRight("+left", "=left", "-left", "+right", "=right", "-right")
		{
			m_btnUpDown.Enable(context);
			m_btnLeftRight.Enable(context);
		}

		void FixedUpdate(const GameObject &owner) override
		{
			//empty
		}

		virtual Phobos::Float_t GetDesiredDixX() override
		{
			return m_btnLeftRight.GetValue() * SPEED;
		}

		virtual Phobos::Float_t GetDesiredDixY() override
		{
			//flip y
			return m_btnUpDown.GetValue() * -SPEED;
		}

	private:
		Phobos::System::AxisButton m_btnUpDown;
		Phobos::System::AxisButton m_btnLeftRight;
};

class AutoController : public Controller
{
	public:
		AutoController() :
			m_fpDirX(SPEED),
			m_fpDirY(SPEED)
		{

		}

		virtual void FixedUpdate(const GameObject &owner) override;

		virtual Phobos::Float_t GetDesiredDixX() override
		{
			return m_fpDirX;
		}

		virtual Phobos::Float_t GetDesiredDixY() override
		{
			return m_fpDirY;
		}

	private:
		Phobos::Float_t m_fpDirX;
		Phobos::Float_t m_fpDirY;
};

class GameObject
{
	public:
		GameObject(std::unique_ptr<Controller> &&controller, Phobos::Float_t x, Phobos::Float_t y, Phobos::UInt32_t color) :
			m_upController(std::move(controller))			
		{
			m_clSprite.m_fpX = x;
			m_clSprite.m_fpY = y;

			m_clSprite.m_u32Color = color;
		}

		GameObject(GameObject &&other) :
			m_upController(std::move(other.m_upController)),
			m_clSprite(std::move(other.m_clSprite))
		{
			//empty
		}

		inline Phobos::Float_t GetX() const
		{
			return m_clSprite.m_fpX;
		}

		inline Phobos::Float_t GetY() const
		{
			return m_clSprite.m_fpY;
		}

		void FixedUpdate(Phobos::System::Seconds ticks)
		{
			m_upController->FixedUpdate(*this);

			m_clSprite.m_fpX += m_upController->GetDesiredDixX() * ticks.count();
			m_clSprite.m_fpY += m_upController->GetDesiredDixY() * ticks.count();
		}

		void Draw()
		{
			auto &render = Render::GetInstance();			

			render.Draw(m_clSprite);
		}

	private:		
		Sprite m_clSprite;

		std::unique_ptr<Controller> m_upController;
};


void AutoController::FixedUpdate(const GameObject &owner)
{
	auto &render = Render::GetInstance();

	if (owner.GetX() >= render.GetWindowWidth() - 10)
	{
		m_fpDirX = -SPEED;
	}

	if (owner.GetY() >= render.GetWindowHeight() - 10)
	{
		m_fpDirY = -SPEED;
	}

	if (owner.GetX() < 0)
	{
		m_fpDirX = SPEED;
	}

	if (owner.GetY() < 0)
	{
		m_fpDirY = SPEED;
	}
}


class GameClient: public Phobos::Engine::Client
{
	public:
		GameClient()			
		{		
			//empty
		}

		void OnConnect() override
		{
			m_vecObjects.emplace_back(std::make_unique<AutoController>(), 150.0f, 150.0f, 0x00FFFF0000);
			m_vecObjects.emplace_back(std::make_unique<PlayerController>(Phobos::Engine::Console::GetInstance()), 150.0f, 250.0f, 0x00FFFFFF00);
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
		std::vector<GameObject> m_vecObjects;
};

void GameClient::OnFixedUpdate()
{	
	if (!Phobos::Engine::GameClock::IsPaused())
	{		
		auto ticks = Phobos::Engine::GameClock::GetFrameDuration();

		for (auto &object : m_vecObjects)
		{
			object.FixedUpdate(ticks);
		}					
	}	

	Render::GetInstance().Clear();

	for (auto &object : m_vecObjects)
	{
		object.Draw();
	}
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
	The toggleTimerPause is created by Core class.

	*/
	session.Bind("kb", "ESCAPE", "quit");
	session.Bind("kb", "p", "toggleClockPause GAME");

	session.Bind("kb", "w", "+up");
	session.Bind("kb", "s", "+down");
	session.Bind("kb", "a", "+left");
	session.Bind("kb", "d", "+right");

	session.Bind("kb", "UP_ARROW", "+up");
	session.Bind("kb", "DOWN_ARROW", "+down");
	session.Bind("kb", "LEFT_ARROW", "+left");
	session.Bind("kb", "RIGHT_ARROW", "+right");

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
