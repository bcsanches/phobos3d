/*
Phobos 3d
November 2014
Copyright (c) 2005-2014 Bruno Sanches  http://github.com/bcsanches/phobos3d

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef PH_CHRONO_H
#define PH_CHRONO_H

#define PH_USE_BOOST_CHRONO

#ifdef PH_USE_BOOST_CHRONO
	#include <boost/chrono.hpp>
#else
	#include <chrono>
#endif

#include <Phobos/Types.h>

namespace Phobos
{
	namespace System
	{
		#ifdef PH_USE_BOOST_CHRONO
			namespace chrono = boost::chrono;
			using boost::ratio;
		#else
			namespace chrono = std::chrono;
			using std::ratio;
		#endif


		typedef chrono::duration<Float_t, ratio<1>> Seconds;

		class Clock
		{
			public:
				typedef Float_t rep;
				typedef ratio<1> period;
				typedef chrono::duration<rep, period> duration;
				typedef chrono::time_point<Clock> time_point;
				static const bool is_monotonic = false;
				static const bool is_steady = false;

				static time_point now() 
				{	
					return time_point(chrono::high_resolution_clock::now().time_since_epoch());
				}
		};		
	}
}

#endif