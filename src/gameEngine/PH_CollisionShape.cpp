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

#include "PH_CollisionShape.h"

#include <Phobos/Exception.h>

#include <iostream>

namespace Phobos
{
	namespace Physics
	{
		CollisionShape::Key_s::Key_s(const BoxShapeInfo_s &info, Float_t physicsScale):
			m_eType(CollisionShapeTypes::BOX),			
			m_pclMesh(NULL)
		{
			m_uShapeInfo.m_stBox = info;

			for(int i = 0;i < 3; ++i)
				m_uShapeInfo.m_stBox.m_v3Dimension[i] *= physicsScale;
		}

		CollisionShape::Key_s::Key_s(CollisionShapeTypes_t type, const CylinderShapeInfo_s &info, Float_t physicsScale):
			m_eType(type),			
			m_pclMesh(NULL)
		{
			if(type == CollisionShapeTypes::CAPSULE)
			{
				if((info.m_fpRadius * 2) > info.m_fpHeight)
					PH_RAISE(INVALID_PARAMETER_EXCEPTION, "CollisionShape::Key_s", "Capsule height must be > than 2 * radius");

				m_uShapeInfo.m_stCylinder.m_fpHeight = (info.m_fpHeight - (2 * info.m_fpRadius))* physicsScale;			
			}
			else
			{
				m_uShapeInfo.m_stCylinder.m_fpHeight = info.m_fpHeight * physicsScale;
			}

			m_uShapeInfo.m_stCylinder.m_fpRadius = info.m_fpRadius * physicsScale;						
		}
				
		CollisionShape::Key_s::Key_s(const Ogre::Mesh &mesh, const Ogre::Vector3 &scale, Float_t physicsScale):
			m_eType(CollisionShapeTypes::MESH),			
			m_pclMesh(&mesh),
			m_v3MeshScale(scale * physicsScale)
		{
			//empty
		}


		CollisionShape::CollisionShape(CollisionShapeTypes_t type):
			m_eType(type)
		{
			//empty
		}

		bool CollisionShape::operator<(const CollisionShape &rhs) const
		{
			return m_eType == rhs.m_eType ? (this->Compare(rhs) < 0) : m_eType < rhs.m_eType;
		}		
	}
}
