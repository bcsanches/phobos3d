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

#include <boost/enable_shared_from_this.hpp>
#include <boost/intrusive/set.hpp>
#include <boost/noncopyable.hpp>

#include <OgrePrerequisites.h>
#include <OgreVector3.h>

#include <PH_Types.h>

#include "PH_CollisionShapeFwd.h"

class btCollisionShape;

namespace Phobos
{
	namespace Physics
	{
		class CollisionShape_c: 
			public boost::intrusive::set_base_hook<boost::intrusive::link_mode<boost::intrusive::auto_unlink> >, 
			public boost::enable_shared_from_this<CollisionShape_c>,
			public boost::noncopyable
		{			
			public:
				enum CollisionShapeTypes_e
				{
					CST_BOX,					
					CST_SPHERE,
					CST_CAPSULE,
					CST_MESH
				};

				struct BoxShapeInfo_s
				{
					Float_t v3Dimension[3];
				};

				struct SphereShapeInfo_s
				{
					Float_t fpRadius;
				};

				struct CapsuleShapeInfo_s
				{
					Float_t fpRadius;
					Float_t fpHeight;
				};

				struct Key_s
				{
					Key_s(const BoxShapeInfo_s &info, Float_t physicsScale);
					Key_s(const CapsuleShapeInfo_s &info, Float_t physicsScale);
					Key_s(const Ogre::Mesh &mesh, const Ogre::Vector3 &scale, Float_t physicsScale);
					
					CollisionShapeTypes_e eType;

					union 
					{
						BoxShapeInfo_s		stBox;
						SphereShapeInfo_s	stSphere;
						CapsuleShapeInfo_s	stCapsule;
					} uShapeInfo;

					//Valid only when eType == CST_MESH
					//We do not insert it on the union because it is an smart pointer (no POD)
					const Ogre::Mesh *pclMesh;
					Ogre::Vector3 v3MeshScale;
				};

				struct KeyComparator_s
				{
					inline bool operator()(const Key_s &lhs, const CollisionShape_c &rhs) const;
					inline bool operator()(const CollisionShape_c &lhs, const Key_s &rhs) const;
				};				

				bool operator<(const CollisionShape_c &rhs) const;

				inline CollisionShapeTypes_e GetType() const;
				
				virtual btCollisionShape &GetCollisionShape() = 0;

			protected:
				CollisionShape_c(CollisionShapeTypes_e type);

				virtual int Compare(const CollisionShape_c &other) const = 0;
				virtual int Compare(const Key_s &other) const = 0;

			private:
				CollisionShapeTypes_e eType;
		};	

		inline CollisionShape_c::CollisionShapeTypes_e CollisionShape_c::GetType() const
		{
			return eType;
		}

		inline bool CollisionShape_c::KeyComparator_s::operator()(const Key_s &lhs, const CollisionShape_c &rhs) const
		{
			if(lhs.eType == rhs.eType)
			{
				return rhs.Compare(lhs) > 0;
			}
			else
			{
				return lhs.eType < rhs.eType;
			}

		}
		
		inline bool CollisionShape_c::KeyComparator_s::operator()(const CollisionShape_c &lhs, const Key_s &rhs) const
		{
			if(lhs.eType == rhs.eType)
			{
				return lhs.Compare(rhs) < 0;
			}
			else
			{
				return lhs.eType < rhs.eType;
			}
		}
	}
}


#endif