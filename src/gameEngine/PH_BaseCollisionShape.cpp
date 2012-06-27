/*
Phobos 3d
June 2012
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

#include "PH_BaseCollisionShape.h"

#include <iostream>

namespace Phobos
{
	namespace Physics
	{
		BaseCollisionShape_c::Key_s::Key_s(const BoxShapeInfo_s &info):
			eType(CST_BOX),			
			pclMesh(NULL)
		{
			uShapeInfo.stBox = info;
		}
				
		BaseCollisionShape_c::Key_s::Key_s(const Ogre::Mesh &mesh, const Ogre::Vector3 &scale):
			eType(CST_MESH),			
			pclMesh(&mesh),
			v3MeshScale(scale)
		{
			//empty
		}


		BaseCollisionShape_c::BaseCollisionShape_c(CollisionShapeTypes_e type):
			eType(type)
		{
			//empty
		}

		bool BaseCollisionShape_c::operator<(const BaseCollisionShape_c &rhs) const
		{
			return eType == rhs.eType ? (this->Compare(rhs) < 0) : eType < rhs.eType;
		}		
	}
}
