/*
Phobos 3d
April 2010
Copyright (c) 2005-2015 Bruno Sanches  http://code.google.com/p/phobos3d

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

#include <Phobos/Engine/EngineMain.h>
#include <Phobos/Memory.h>

extern "C" int SDL_main(int argc, char *argv[])
{
	#ifdef PH_DEBUG
		Phobos::EnableMemoryTracker();

		//Phobos::BreakMemoryAllocation(405);
	#endif

	{
		Phobos::Engine::EngineMain engine(argc, argv);

	#ifndef PH_DEBUG
		try
	#endif
		{
			engine.StartMainLoop();
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
