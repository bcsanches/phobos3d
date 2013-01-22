/*
Phobos 3d
February 2012
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

#ifndef PH_SYSTEM_TIMER_H
#define PH_SYSTEM_TIMER_H

#include "Phobos/System/SystemAPI.h"
#include "PH_Types.h"

namespace Phobos
{
	namespace System
	{
		class PH_SYSTEM_API Timer_c
		{
			private:
				UInt_t uLastTime;
				Float_t fpMinInterval;


			public:
				Timer_c();

				Float_t Elapsed();

				void Reset();

				/**
					Sets the minimum interval time for elapsed in seconds.

					The Elapsed function will always return a number >= fpMinInterval or zero otherwise.

				*/
				inline void SetMinInterval(Float_t minInterval)
				{
					fpMinInterval = minInterval;
				}
		};
	}
}

#endif