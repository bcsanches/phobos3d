/*
Phobos 3d
January 2010
Copyright (c) 2005-2013 Bruno Sanches  http://code.google.com/p/phobos3d

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef PH_BASE_API_H
#define PH_BASE_API_H

#include "Phobos/Defs.h"

#ifdef PH_WIN32

	#ifdef PH_BASE_EXPORTS
		#define PH_BASE_API __declspec(dllexport)

		#define PH_BASE_API_PROC extern "C" PH_BASE_API

		#define PH_STDCALL __stdcall

	#else

		#define PH_BASE_API
		#define PH_BASE_API_PROC extern "C" PH_BASE_API

	#endif
#else
	#error "Unknown platform"
#endif

#endif
