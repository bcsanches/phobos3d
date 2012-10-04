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

#include "PH_Memory.h"

#include <PH_Exception.h>

void Phobos::DumpMemoryLeaks()
{
	#if (defined PH_WIN32)
		#if (defined _DEBUG)
			_CrtDumpMemoryLeaks();			
		#endif
	#endif
}

void Phobos::EnableMemoryTracker()
{
	#if (defined PH_WIN32)
		#if (defined _DEBUG)			
			_CrtSetDbgFlag( _CRTDBG_REPORT_FLAG | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
			_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
			_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);			
		#endif
	#endif
}

bool Phobos::BreakMemoryAllocation(long id)
{
	#if (defined PH_WIN32)
		#if (defined _DEBUG)
			int *p = (int*) malloc(sizeof(int));
			long blockNumber;
			_CrtIsMemoryBlock(p, sizeof(int), &blockNumber, NULL, NULL);

			free(p);

			if(blockNumber >= id)
			{
				return false;
			}

			_CrtSetBreakAlloc(id);
		#endif
	#endif

	return true;
}
