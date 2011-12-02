/*
Phobos 3d
October 2011
Copyright (c) 2005-2011 Bruno Sanches  http://code.google.com/p/phobos3d

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef PH_PHYSICS_UTIL_H
#define PH_PHYSICS_UTIL_H

#include <btBulletDynamicsCommon.h>

#include <PH_Transform.h>

namespace Phobos
{
	namespace Physics
	{
		inline btVector3 MakeVector3(const Ogre::Vector3 &v)
		{
			return btVector3(v.x, v.y, v.z);
		}

		inline Ogre::Vector3 MakeVector3(const btVector3 &v)
		{
			return Ogre::Vector3(v.x(), v.y(), v.z());
		}

		inline Ogre::Quaternion MakeQuaternion(const btQuaternion &q)
		{
			return Ogre::Quaternion(q.w(), q.x(), q.y(), q.z());
		}

		inline btQuaternion MakeQuaternion(const Ogre::Quaternion &q)
		{
			return btQuaternion(q.x, q.y, q.z, q.w);
		}

		inline btTransform MakeTransform(const Transform_c &transform)
		{
			return btTransform(MakeQuaternion(transform.GetRotation()), MakeVector3(transform.GetOrigin()));
		}				

		inline Transform_c MakeTransform(const btTransform &transform)
		{
			return Transform_c(MakeVector3(transform.getOrigin()), MakeQuaternion(transform.getRotation()));
		}
	}
}

#endif