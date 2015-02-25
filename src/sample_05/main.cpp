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

#include "Render.h"

#include <Phobos/Engine/Console.h>
#include <Phobos/Engine/Session.h>
#include <Phobos/Engine/Kernel.h>

int main(int argc, char **argv)
{		
	Phobos::Engine::Kernel engine(argc, argv,
	{
		Phobos::Engine::MakeLocalModuleClass("Console", Phobos::Engine::Console::CreateInstance, 100),
		Phobos::Engine::MakeLocalModuleClass("Session", Phobos::Engine::Session::CreateInstance, 200),
		Phobos::Engine::MakeLocalModuleClass("Render", Render::CreateInstance, 500)
	});

	auto &session = Phobos::Engine::Session::GetInstance();

	//
	//The ideal is to store those on a config, perhaps autoexec.cfg, see sample 05
	//

	/*
	Bind ESCAPE key from keyboard (kb) with the command quit

	The quit command is automatically created by the engine core (Core class) and it stops the main loop
	The toggleTimerPause is created by Core class.

	*/
	session.Bind("kb", "ESCAPE", "quit");

	/*
	By default the console starts open (even if not visible, as the default console), so tell the session that we do not want it now
	*/
	session.CloseConsole();

	engine.RunMainLoop();	

	return 0;
}
