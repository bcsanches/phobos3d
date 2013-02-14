#include "PH_CollisionShapes.h"

#include <OgreMesh.h>
#include <OgreVector3.h>

#include <Phobos/Error.h>
#include <Phobos/Exception.h>

#include "PH_CollisionMesh.h"
#include "PH_PhysicsConv.h"

namespace Phobos
{
	namespace Physics
	{
		inline int CompareVectors(const Float_t a[], const Float_t b[])
		{
			for(int i = 0;i < 3; ++i)
			{
				if(a[i] == b[i])
					continue;

				return a[i] < b[i] ? -1 : 1;
			}

			return 0;
		}

		BoxCollisionShape::BoxCollisionShape(const Ogre::Vector3 &dimensions, Float_t physicsScale):
			CollisionShape(CollisionShapeTypes::BOX),
			m_clBoxShape(MakeVector3(dimensions, physicsScale) * 0.5f)
		{
			//empty
		}

		int BoxCollisionShape::Compare(const CollisionShape &other) const
		{
			PH_ASSERT(other.GetType() == this->GetType());

			const BoxCollisionShape &bbox = static_cast<const BoxCollisionShape&>(other);

			return CompareVectors(m_clBoxShape.getHalfExtentsWithoutMargin(), bbox.m_clBoxShape.getHalfExtentsWithoutMargin());
		}				

		int BoxCollisionShape::Compare(const Key_s &other) const
		{			
			const btVector3 extents = this->m_clBoxShape.getHalfExtentsWithoutMargin() * 2;

			return CompareVectors(m_clBoxShape.getHalfExtentsWithoutMargin(), extents);
		}

		btCollisionShape &BoxCollisionShape::GetCollisionShape()
		{
			return m_clBoxShape;
		}

		//
		//
		//
		//
		//
		
		CapsuleCollisionShape::CapsuleCollisionShape(Float_t radius, Float_t height, Float_t physicsScale):
			CollisionShape(CollisionShapeTypes::CAPSULE),
			m_clCapsuleShape(radius * physicsScale, (height - (2 * radius))* physicsScale)
		{
			if((radius * 2) > height)
				PH_RAISE(INVALID_PARAMETER_EXCEPTION, "CollisionShape::Key_s", "Capsule height must be > than 2 * radius");
		}


		btCollisionShape &CapsuleCollisionShape::GetCollisionShape()		
		{
			return m_clCapsuleShape;
		}
			
		int CapsuleCollisionShape::Compare(const CollisionShape &other) const
		{
			PH_ASSERT(other.GetType() == this->GetType());

			const CapsuleCollisionShape &capsule = static_cast<const CapsuleCollisionShape&>(other);

			Float_t left = m_clCapsuleShape.getRadius();
			Float_t right = capsule.m_clCapsuleShape.getRadius();

			if(left == right)
			{
				left = m_clCapsuleShape.getHalfHeight();
				right = capsule.m_clCapsuleShape.getHalfHeight();
			}

			if(left < right)
				return -1;
			else if(left == right)
				return 0;
			else
				return 1;

		}

		int CapsuleCollisionShape::Compare(const Key_s &other) const
		{
			Float_t left = m_clCapsuleShape.getRadius();
			Float_t right = other.m_uShapeInfo.m_stCylinder.m_fpRadius;

			if(left == right)
			{
				left = m_clCapsuleShape.getHalfHeight() * 2;
				right = other.m_uShapeInfo.m_stCylinder.m_fpHeight;
			}

			if(left < right)
				return -1;
			else if(left == right)
				return 0;
			else
				return 1;
		}

		//
		//
		//
		//
		//

		
		ScaledMeshCollissionShape::ScaledMeshCollissionShape(CollisionMeshPtr_t collisionMesh, const Ogre::Vector3 &localScale, Float_t physicsScale):
			CollisionShape(CollisionShapeTypes::MESH),
			m_spOriginalMesh(collisionMesh),
			m_clScaleTriangleMeshShape(&collisionMesh->GetTriangleMeshShape(), MakeVector3(localScale, physicsScale))
		{
			//empty
		}

		int ScaledMeshCollissionShape::Compare(const CollisionShape &other) const
		{
			PH_ASSERT(other.GetType() == this->GetType());			

			const ScaledMeshCollissionShape &otherMesh = static_cast<const ScaledMeshCollissionShape&>(other);

			int cmp = this->m_spOriginalMesh->GetName().compare(otherMesh.m_spOriginalMesh->GetName());
			if(cmp == 0)
			{
				return CompareVectors(this->m_clScaleTriangleMeshShape.getLocalScaling(), otherMesh.m_clScaleTriangleMeshShape.getLocalScaling());
			}			
			else
				return cmp;
		}				

		int ScaledMeshCollissionShape::Compare(const Key_s &other) const
		{			
			PH_ASSERT(other.m_eType == this->GetType());						

			int cmp = this->m_spOriginalMesh->GetName().compare(other.m_pclMesh->getName());
			if(cmp == 0)
			{
				return CompareVectors(this->m_clScaleTriangleMeshShape.getLocalScaling(), btVector3(other.m_v3MeshScale.x, other.m_v3MeshScale.y, other.m_v3MeshScale.z));
			}			
			else
				return cmp;
		}

		btCollisionShape &ScaledMeshCollissionShape::GetCollisionShape()
		{
			return m_clScaleTriangleMeshShape;
		}
			
		//
		//
		//
		//
		//

		template <typename T>
		int CompareCylinders(const T &lhs, const T &rhs)
		{
			Float_t left = lhs.getRadius();
			Float_t right = rhs.getRadius();

			if(left == right)
			{
				const int upAxis = lhs.getUpAxis();				

				//get with margin, because radius also use margin
				left = lhs.getHalfExtentsWithMargin()[upAxis] * 2;
				right = rhs.getHalfExtentsWithMargin()[upAxis];
			}

			if(left < right)
				return -1;
			else if(left == right)
				return 0;
			else
				return 1;
		}

		template <typename T>
		int CompareCylinders(const T &lhs, const CollisionShape::Key_s &rhs)
		{
			Float_t left = lhs.getRadius();
			Float_t right = rhs.m_uShapeInfo.m_stCylinder.m_fpRadius;

			if(left == right)
			{
				const int upAxis = lhs.getUpAxis();				

				//get with margin, because radius also use margin
				left = lhs.getHalfExtentsWithMargin()[upAxis] * 2;
				right = rhs.m_uShapeInfo.m_stCylinder.m_fpHeight;
			}

			if(left < right)
				return -1;
			else if(left == right)
				return 0;
			else
				return 1;
		}

		//
		//
		//
		//
		//

		CylinderCollisionShapeX::CylinderCollisionShapeX(Float_t radius, Float_t height, Float_t physicsScale):
			CollisionShape(CollisionShapeTypes::CYLINDER_X),
			m_clCylinderShape(Physics::MakeVector3(Ogre::Vector3(height / 2.0f, radius, radius), physicsScale))
		{
			//empty
		}


		btCollisionShape &CylinderCollisionShapeX::GetCollisionShape()
		{
			return m_clCylinderShape;
		}

		int CylinderCollisionShapeX::Compare(const CollisionShape &other) const
		{
			PH_ASSERT(other.GetType() == this->GetType());

			const CylinderCollisionShapeX &cylinder = static_cast<const CylinderCollisionShapeX&>(other);

			return CompareCylinders(m_clCylinderShape, cylinder.m_clCylinderShape);
		}

		int CylinderCollisionShapeX::Compare(const Key_s &other) const
		{
			return CompareCylinders(m_clCylinderShape, other);
		}

		//
		//
		//
		//
		//
		
		CylinderCollisionShapeY::CylinderCollisionShapeY(Float_t radius, Float_t height, Float_t physicsScale):
			CollisionShape(CollisionShapeTypes::CYLINDER_Y),
			m_clCylinderShape(Physics::MakeVector3(Ogre::Vector3(radius, height / 2.0f, radius), physicsScale))
		{
			//empty
		}


		btCollisionShape &CylinderCollisionShapeY::GetCollisionShape()
		{
			return m_clCylinderShape;
		}

		int CylinderCollisionShapeY::Compare(const CollisionShape &other) const
		{
			PH_ASSERT(other.GetType() == this->GetType());

			const CylinderCollisionShapeY &cylinder = static_cast<const CylinderCollisionShapeY&>(other);

			return CompareCylinders(m_clCylinderShape, cylinder.m_clCylinderShape);
		}

		int CylinderCollisionShapeY::Compare(const Key_s &other) const
		{
			return CompareCylinders(m_clCylinderShape, other);
		}

		//
		//
		//
		//
		//

		CylinderCollisionShapeZ::CylinderCollisionShapeZ(Float_t radius, Float_t height, Float_t physicsScale):
			CollisionShape(CollisionShapeTypes::CYLINDER_Z),
			m_clCylinderShape(Physics::MakeVector3(Ogre::Vector3(radius, radius, height / 2.0f), physicsScale))
		{
			//empty
		}


		btCollisionShape &CylinderCollisionShapeZ::GetCollisionShape()
		{
			return m_clCylinderShape;
		}

		int CylinderCollisionShapeZ::Compare(const CollisionShape &other) const
		{
			PH_ASSERT(other.GetType() == this->GetType());

			const CylinderCollisionShapeZ &cylinder = static_cast<const CylinderCollisionShapeZ&>(other);

			return CompareCylinders(m_clCylinderShape, cylinder.m_clCylinderShape);
		}

		int CylinderCollisionShapeZ::Compare(const Key_s &other) const
		{
			return CompareCylinders(m_clCylinderShape, other);
		}
	}
}
