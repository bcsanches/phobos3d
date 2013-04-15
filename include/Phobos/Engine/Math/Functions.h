/*
Phobos 3d
October 2010
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

#ifndef PH_ENGINE_MATH_FUNCTIONS_H
#define PH_ENGINE_MATH_FUNCTIONS_H

#include <float.h>
#include <algorithm>

#include <Phobos/Types.h>

#define PH_FLOAT_EPSILON FLT_EPSILON

namespace Phobos
{	
	namespace Engine
	{
		namespace Math
		{
			template <typename T>
			inline T Lerp(const T &a, const T &b, Float_t alpha)
			{
				//Calculates a + alpha(b - a)
				T temp(b - a);

				temp *= alpha;

				return(a + temp);	
			}

			template <typename T>
			inline T Interpolate(const T &a, const T & b, Float_t alpha)
			{
				return(Lerp<T>(a, b, alpha));
			}	

			template <typename T>
			inline T Clamp(const T value, const T minV, const T maxV)
			{
				//return(std::max(std::min(value, maxV), minV));
				return (value < minV) ? minV : (value > maxV) ? maxV : value;
			} 	

			template <typename T>
			inline T Positive(const T v1)
			{
				return std::abs(v1, -v1);
			}	
		}
	}
}

#endif
