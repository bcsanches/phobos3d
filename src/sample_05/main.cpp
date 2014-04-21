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
	* Make sure your IDE is pointing to the "work/sample05" as working directory, so the sample can read the "autoexec.cfg"
	* Configure your IDE to allow the program to find the dlls on bin/

For Visual Studio users, go to the project Property Pages, on the "Debugging" page, sets:
	* Working directory: ..\..\..\work\sample05
	* Environment: PATH=$(OutDir);$(SDL_HOME)\VisualC\SDL\Debug\

	* Note that "$(SDL_HOME)\VisualC\SDL\Debug\" should point to a folder where SDL.dll is located
*/

#include <sstream>

#include "Console.h"
#include "Render.h"

#include <Phobos/Engine/Session.h>
#include <Phobos/Shell/Variable.h>
#include <Phobos/Shell/Utils.h>
#include <Phobos/Engine/Core.h>
#include <Phobos/Memory.h>
#include <Phobos/ProcVector.h>

#include <Phobos/System/Timer.h>

class EngineMain
{
	public:
		EngineMain();
		~EngineMain();

		void MainLoop(void);					

	private:			
		Phobos::ProcVector	m_clSingletons;
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
