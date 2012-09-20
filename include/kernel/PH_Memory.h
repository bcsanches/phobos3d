/*
Phobos 3d
September 2012
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

#ifndef PH_MEMORY_H
#define PH_MEMORY_H

#include <PH_Defs.h>
#include <PH_KernelAPI.h>

#if (defined PH_WIN32)
	#if (defined _DEBUG)
		#define _CRTDBG_MAP_ALLOC
		#include <stdlib.h>
		#include <crtdbg.h>

		#ifndef PH_NEW      
			#define PH_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )      				
		#endif		
	#else
		#define PH_NEW new
	#endif
#endif

namespace Phobos
{
	PH_KERNEL_API_PROC void DumpMemoryLeaks();
	PH_KERNEL_API_PROC void EnableMemoryTracker();
	PH_KERNEL_API_PROC bool BreakMemoryAllocation(long id);
}

#endif
