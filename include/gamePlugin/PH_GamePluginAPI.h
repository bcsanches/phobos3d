/*
Phobos 3d
February 2010
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

#ifndef PH_GAME_PLUGIN_API_H
#define PH_GAME_PLUGIN_API_H

#include <PH_Defs.h>

#ifdef PH_WIN32
	#ifdef PH_GAME_PLUGIN_EXPORTS
		#define PH_GAME_PLUGIN_API __declspec(dllexport)
	#else
		#define PH_GAME_PLUGIN_API
	#endif

	#define PH_GAME_PLUGIN_API_PROC extern "C" PH_GAME_PLUGIN_API	
	#define PH_GAME_PLUGIN_ENTRY_POINT_PROC extern "C" __declspec(dllexport)
#elif defined PH_LINUX
	#define PH_GAME_PLUGIN_API
	#define PH_GAME_PLUGIN_API_PROC extern "C"
	#define PH_GAME_PLUGIN_ENTRY_POINT_PROC extern "C"
#else
	#error "Platform not defined"
#endif

#endif
