/*
Phobos 3d
July 2012
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

#ifndef PH_OGRE_MATH_FUNCTIONS_H
#define PH_OGRE_MATH_FUNCTIONS_H

#include <float.h>
#include <algorithm>

#include <OgreQuaternion.h>
#include <OgreMath.h>

#include <Math/PH_Functions.h>
#include <Phobos/Types.h>

#define PH_FLOAT_EPSILON FLT_EPSILON

namespace Phobos
{	
	namespace Math
	{
		template <>
		inline Ogre::Quaternion Interpolate(const Ogre::Quaternion &a, const Ogre::Quaternion &b, Float_t alpha)
		{
			return(Ogre::Quaternion::Slerp(alpha, a, b, true));
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
}

#endif
