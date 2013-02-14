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

#ifndef PH_COLLISION_SHAPE_H
#define PH_COLLISION_SHAPE_H

#include <boost/intrusive/set.hpp>

#include <OgrePrerequisites.h>
#include <OgreVector3.h>

#include <Phobos/DisableCopy.h>
#include <Phobos/Types.h>

#include "PH_CollisionShapeFwd.h"

class btCollisionShape;

namespace Phobos
{
	namespace Physics
	{
		enum class CollisionShapeTypes
		{			
			BOX,					
			SPHERE,
			CAPSULE,
			CYLINDER_X,
			CYLINDER_Y,
			CYLINDER_Z,
			MESH			
		};

		typedef CollisionShapeTypes CollisionShapeTypes_t;

		class CollisionShape: 
			public boost::intrusive::set_base_hook<boost::intrusive::link_mode<boost::intrusive::auto_unlink> >, 
			public std::enable_shared_from_this<CollisionShape>	
		{			
			PH_DISABLE_COPY(CollisionShape);

			public:				
				struct BoxShapeInfo_s
				{
					Float_t m_v3Dimension[3];
				};

				struct SphereShapeInfo_s
				{
					Float_t m_fpRadius;
				};

				struct CylinderShapeInfo_s
				{
					Float_t m_fpRadius;
					Float_t m_fpHeight;
				};

				struct Key_s
				{
					Key_s(const BoxShapeInfo_s &info, Float_t physicsScale);
					Key_s(CollisionShapeTypes_t type, const CylinderShapeInfo_s &info, Float_t physicsScale);
					Key_s(const Ogre::Mesh &mesh, const Ogre::Vector3 &scale, Float_t physicsScale);
					
					CollisionShapeTypes_t m_eType;

					union 
					{
						BoxShapeInfo_s		m_stBox;
						SphereShapeInfo_s	m_stSphere;
						CylinderShapeInfo_s	m_stCylinder;
					} m_uShapeInfo;

					//Valid only when eType == CST_MESH
					//We do not insert it on the union because it is an smart pointer (no POD)
					const Ogre::Mesh *m_pclMesh;
					Ogre::Vector3 m_v3MeshScale;
				};

				struct KeyComparator_s
				{
					inline bool operator()(const Key_s &lhs, const CollisionShape &rhs) const;
					inline bool operator()(const CollisionShape &lhs, const Key_s &rhs) const;
				};				

				bool operator<(const CollisionShape &rhs) const;

				inline CollisionShapeTypes_t GetType() const;
				
				virtual btCollisionShape &GetCollisionShape() = 0;

			protected:
				CollisionShape(CollisionShapeTypes_t type);

				virtual int Compare(const CollisionShape &other) const = 0;
				virtual int Compare(const Key_s &other) const = 0;

			private:
				CollisionShapeTypes_t m_eType;
		};	

		inline CollisionShapeTypes_t CollisionShape::GetType() const
		{
			return m_eType;
		}

		inline bool CollisionShape::KeyComparator_s::operator()(const Key_s &lhs, const CollisionShape &rhs) const
		{
			if(lhs.m_eType == rhs.m_eType)
			{
				return rhs.Compare(lhs) > 0;
			}
			else
			{
				return lhs.m_eType < rhs.m_eType;
			}

		}
		
		inline bool CollisionShape::KeyComparator_s::operator()(const CollisionShape &lhs, const Key_s &rhs) const
		{
			if(lhs.m_eType == rhs.m_eType)
			{
				return lhs.Compare(rhs) < 0;
			}
			else
			{
				return lhs.m_eType < rhs.m_eType;
			}
		}
	}
}


#endif