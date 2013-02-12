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

#ifndef PH_PHYSICS_CONV_H
#define PH_PHYSICS_CONV_H

#include <btBulletDynamicsCommon.h>

#include <PH_Transform.h>

namespace Phobos
{
	namespace Physics
	{
		inline btVector3 MakeVector3(const Ogre::Vector3 &v, const Float_t scale)
		{
			return btVector3(v.x, v.y, v.z) * scale;
		}

		inline Ogre::Vector3 MakeVector3(const btVector3 &v, Float_t physicsToGameScale)
		{
			return Ogre::Vector3(v.x(), v.y(), v.z()) * physicsToGameScale;
		}

		inline Ogre::Quaternion MakeQuaternion(const btQuaternion &q)
		{
			return Ogre::Quaternion(q.w(), q.x(), q.y(), q.z());
		}

		inline btQuaternion MakeQuaternion(const Ogre::Quaternion &q)
		{
			return btQuaternion(q.x, q.y, q.z, q.w);
		}

		inline btTransform MakeTransform(const Transform &transform, const Float_t scale)
		{
			return btTransform(MakeQuaternion(transform.GetRotation()), MakeVector3(transform.GetOrigin(), scale));
		}				

		inline Transform MakeTransform(const btTransform &transform , Float_t physicsToGameScale)
		{
			return Transform(MakeVector3(transform.getOrigin(), physicsToGameScale), MakeQuaternion(transform.getRotation()));
		}
	}
}

#endif