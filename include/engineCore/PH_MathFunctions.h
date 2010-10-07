/*
Phobos 3d
  October 2010

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

#ifndef PH_MATH_FUNCTIONS_H
#define PH_MATH_FUNCTIONS_H

#include <float.h>
#include <algorithm>

#include <OgreQuaternion.h>
#include <OgreMath.h>
#include <PH_Types.h>

#define PH_FLOAT_EPSILON FLT_EPSILON

namespace Phobos
{	
	template <typename T>
	inline T MathLerp(const T &a, const T &b, Float_t alpha)
	{
		//Calculates a + alpha(b - a)
		T temp(b - a);

		temp *= alpha;

		return(a + temp);	
	}

	template <typename T>
	inline T MathInterpolate(const T &a, const T & b, Float_t alpha)
	{
		return(MathLerp<T>(a, b, alpha));
	}

	template <>
	inline Ogre::Quaternion MathInterpolate(const Ogre::Quaternion &a, const Ogre::Quaternion &b, Float_t alpha)
	{
		return(Ogre::Quaternion::Slerp(alpha, a, b, true));
	}

	template <typename T>
	inline T MathClamp(const T value, const T minV, const T maxV)
	{
		//return(std::max(std::min(value, maxV), minV));
		return (value < minV) ? minV : (value > maxV) ? maxV : value;
	} 	

	template <typename T>
	inline T MathPositive(const T v1)
	{
		return std::max(v1, -v1);
	}

	inline Ogre::Degree ClipDegree(Ogre::Degree angle)
	{
		const Ogre::Real r360(360);
		if(angle >= Ogre::Degree(r360))
		{
			int div = (int)(angle.valueDegrees() / 360.0f);
			angle -= Ogre::Degree(r360) * Ogre::Real(div);
		}
		else if (angle < Ogre::Degree(0))
		{
			int div = (int)(-angle.valueDegrees() / 360.0f);
			angle += Ogre::Degree(360 * Ogre::Real(div + 1));
		}

		return angle;
	}
}

#endif
