/*
Phobos 3d
  June 2011

  Copyright (C) 2005-2011 Bruno Crivelari Sanches

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Bruno Crivelari Sanches bcsanches@gmail.com
*/

#ifndef PH_ATTRIBUTE_SYSTEM_API_H
#define PH_ATTRIBUTE_SYSTEM_API_H

#ifdef PH_WIN32
	#ifdef PH_ATTRIBUTE_SYSTEM_EXPORTS
		#define PH_ATTRIBUTE_SYSTEM_API __declspec(dllexport)
	#else
		#define PH_ATTRIBUTE_SYSTEM_API
	#endif

	#define PH_ATTRIBUTE_SYSTEM_API_PROC extern "C" PH_ATTRIBUTE_SYSTEM_API
#else

    #ifdef PH_LINUX
        #define PH_ATTRIBUTE_SYSTEM_API
    #else
        #error "Platform not defined"
    #endif

#endif

#endif
