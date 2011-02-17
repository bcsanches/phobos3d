/*
Phobos 3d
  version 0.0.1, January 2010

  Copyright (C) 2005-2010 Bruno Crivelari Sanches

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

#ifndef PH_DEFS_H
#define PH_DEFS_H

#ifdef PH_MSVC
	//Template exportation (stl errors on dll)
	#pragma warning (disable : 4251)
	#pragma warning (disable : 4275)
#endif

#define PH_FORCE_ENUM_32_BITS 0xFFFFFFFF

	/*
	#ifndef _WIN32_WINNT
	#error "Phobos: please define _WIN32_WINNT=0x0400 at minimun."
	#endif
	*/

	#if ((!defined PH_WIN32) && (!defined PH_LINUX))
	#error "Phobos: please define PH_WIN32 or PH_LINUX"
	#endif

    #if ((!defined PH_DEBUG) && (!defined PH_RELEASE))
	#error "Phobos: please define PH_DEBUG or PH_RELEASE"
	#endif

	#if ((!defined PH_CHECK_ASSERT) && (!defined PH_IGNORE_ASSERT))
	#error "Phobos: please define PH_CHECK_ASSERT or PH_IGNORE_ASSERT"
	#endif

	#if ((!defined PH_MSVC) && (!defined PH_GCC))
	#error "Phobos: please define PH_MSVC or PH_GCC"
	#endif

#endif
