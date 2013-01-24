/*
Phobos 3d
August 2012
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

#ifndef PH_PHYSICS_UTIL_H
#define PH_PHYSICS_UTIL_H

#include <OgrePrerequisites.h>

#include <PH_Types.h>

#include <Phobos/Register/TableFwd.h>

#include "PH_CollisionTagFwd.h"
#include "PH_CollisionShapeFwd.h"

namespace Phobos
{
	class Transform_c;

	namespace Physics
	{
		namespace Utils
		{			
			CollisionShapePtr_t CreateCollisionShape(const Register::Table_c &collisionDef, const Ogre::Vector3 &scale);
		}
	}
}

#endif