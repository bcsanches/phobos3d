#include "PH_CollisionShapes.h"

#include <OgreMesh.h>
#include <OgreVector3.h>

#include <PH_Error.h>
#include <PH_Exception.h>

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

		BoxCollisionShape_c::BoxCollisionShape_c(const Ogre::Vector3 &dimensions, Float_t physicsScale):
			CollisionShape_c(CollisionShapeTypes::BOX),
			clBoxShape(MakeVector3(dimensions, physicsScale) * 0.5f)
		{
			//empty
		}

		int BoxCollisionShape_c::Compare(const CollisionShape_c &other) const
		{
			PH_ASSERT(other.GetType() == this->GetType());

			const BoxCollisionShape_c &bbox = static_cast<const BoxCollisionShape_c&>(other);

			return CompareVectors(clBoxShape.getHalfExtentsWithoutMargin(), bbox.clBoxShape.getHalfExtentsWithoutMargin());
		}				

		int BoxCollisionShape_c::Compare(const Key_s &other) const
		{			
			const btVector3 extents = this->clBoxShape.getHalfExtentsWithoutMargin() * 2;

			return CompareVectors(clBoxShape.getHalfExtentsWithoutMargin(), extents);
		}

		btCollisionShape &BoxCollisionShape_c::GetCollisionShape()
		{
			return clBoxShape;
		}

		//
		//
		//
		//
		//
		
		CapsuleCollisionShape_c::CapsuleCollisionShape_c(Float_t radius, Float_t height, Float_t physicsScale):
			CollisionShape_c(CollisionShapeTypes::CAPSULE),
			clCapsuleShape(radius * physicsScale, (height - (2 * radius))* physicsScale)
		{
			if((radius * 2) > height)
				PH_RAISE(INVALID_PARAMETER_EXCEPTION, "CollisionShape_c::Key_s", "Capsule height must be > than 2 * radius");
		}


		btCollisionShape &CapsuleCollisionShape_c::GetCollisionShape()		
		{
			return clCapsuleShape;
		}
			
		int CapsuleCollisionShape_c::Compare(const CollisionShape_c &other) const
		{
			PH_ASSERT(other.GetType() == this->GetType());

			const CapsuleCollisionShape_c &capsule = static_cast<const CapsuleCollisionShape_c&>(other);

			Float_t left = clCapsuleShape.getRadius();
			Float_t right = capsule.clCapsuleShape.getRadius();

			if(left == right)
			{
				left = clCapsuleShape.getHalfHeight();
				right = capsule.clCapsuleShape.getHalfHeight();
			}

			if(left < right)
				return -1;
			else if(left == right)
				return 0;
			else
				return 1;

		}

		int CapsuleCollisionShape_c::Compare(const Key_s &other) const
		{
			Float_t left = clCapsuleShape.getRadius();
			Float_t right = other.uShapeInfo.stCylinder.fpRadius;

			if(left == right)
			{
				left = clCapsuleShape.getHalfHeight() * 2;
				right = other.uShapeInfo.stCylinder.fpHeight;
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

		
		ScaledMeshCollissionShape_c::ScaledMeshCollissionShape_c(CollisionMeshPtr_t collisionMesh, const Ogre::Vector3 &localScale, Float_t physicsScale):
			CollisionShape_c(CollisionShapeTypes::MESH),
			spOriginalMesh(collisionMesh),
			clScaleTriangleMeshShape(&collisionMesh->GetTriangleMeshShape(), MakeVector3(localScale, physicsScale))
		{
			//empty
		}

		int ScaledMeshCollissionShape_c::Compare(const CollisionShape_c &other) const
		{
			PH_ASSERT(other.GetType() == this->GetType());			

			const ScaledMeshCollissionShape_c &otherMesh = static_cast<const ScaledMeshCollissionShape_c&>(other);

			int cmp = this->spOriginalMesh->GetName().compare(otherMesh.spOriginalMesh->GetName());
			if(cmp == 0)
			{
				return CompareVectors(this->clScaleTriangleMeshShape.getLocalScaling(), otherMesh.clScaleTriangleMeshShape.getLocalScaling());
			}			
			else
				return cmp;
		}				

		int ScaledMeshCollissionShape_c::Compare(const Key_s &other) const
		{			
			PH_ASSERT(other.eType == this->GetType());						

			int cmp = this->spOriginalMesh->GetName().compare(other.pclMesh->getName());
			if(cmp == 0)
			{
				return CompareVectors(this->clScaleTriangleMeshShape.getLocalScaling(), btVector3(other.v3MeshScale.x, other.v3MeshScale.y, other.v3MeshScale.z));
			}			
			else
				return cmp;
		}

		btCollisionShape &ScaledMeshCollissionShape_c::GetCollisionShape()
		{
			return clScaleTriangleMeshShape;
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
		int CompareCylinders(const T &lhs, const CollisionShape_c::Key_s &rhs)
		{
			Float_t left = lhs.getRadius();
			Float_t right = rhs.uShapeInfo.stCylinder.fpRadius;

			if(left == right)
			{
				const int upAxis = lhs.getUpAxis();				

				//get with margin, because radius also use margin
				left = lhs.getHalfExtentsWithMargin()[upAxis] * 2;
				right = rhs.uShapeInfo.stCylinder.fpHeight;
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

		CylinderCollisionShapeX_c::CylinderCollisionShapeX_c(Float_t radius, Float_t height, Float_t physicsScale):
			CollisionShape_c(CollisionShapeTypes::CYLINDER_X),
			clCylinderShape(Physics::MakeVector3(Ogre::Vector3(height / 2.0f, radius, radius), physicsScale))
		{
			//empty
		}


		btCollisionShape &CylinderCollisionShapeX_c::GetCollisionShape()
		{
			return clCylinderShape;
		}

		int CylinderCollisionShapeX_c::Compare(const CollisionShape_c &other) const
		{
			PH_ASSERT(other.GetType() == this->GetType());

			const CylinderCollisionShapeX_c &cylinder = static_cast<const CylinderCollisionShapeX_c&>(other);

			return CompareCylinders(clCylinderShape, cylinder.clCylinderShape);
		}

		int CylinderCollisionShapeX_c::Compare(const Key_s &other) const
		{
			return CompareCylinders(clCylinderShape, other);
		}

		//
		//
		//
		//
		//
		
		CylinderCollisionShapeY_c::CylinderCollisionShapeY_c(Float_t radius, Float_t height, Float_t physicsScale):
			CollisionShape_c(CollisionShapeTypes::CYLINDER_Y),
			clCylinderShape(Physics::MakeVector3(Ogre::Vector3(radius, height / 2.0f, radius), physicsScale))
		{
			//empty
		}


		btCollisionShape &CylinderCollisionShapeY_c::GetCollisionShape()
		{
			return clCylinderShape;
		}

		int CylinderCollisionShapeY_c::Compare(const CollisionShape_c &other) const
		{
			PH_ASSERT(other.GetType() == this->GetType());

			const CylinderCollisionShapeY_c &cylinder = static_cast<const CylinderCollisionShapeY_c&>(other);

			return CompareCylinders(clCylinderShape, cylinder.clCylinderShape);
		}

		int CylinderCollisionShapeY_c::Compare(const Key_s &other) const
		{
			return CompareCylinders(clCylinderShape, other);
		}

		//
		//
		//
		//
		//

		CylinderCollisionShapeZ_c::CylinderCollisionShapeZ_c(Float_t radius, Float_t height, Float_t physicsScale):
			CollisionShape_c(CollisionShapeTypes::CYLINDER_Z),
			clCylinderShape(Physics::MakeVector3(Ogre::Vector3(radius, radius, height / 2.0f), physicsScale))
		{
			//empty
		}


		btCollisionShape &CylinderCollisionShapeZ_c::GetCollisionShape()
		{
			return clCylinderShape;
		}

		int CylinderCollisionShapeZ_c::Compare(const CollisionShape_c &other) const
		{
			PH_ASSERT(other.GetType() == this->GetType());

			const CylinderCollisionShapeZ_c &cylinder = static_cast<const CylinderCollisionShapeZ_c&>(other);

			return CompareCylinders(clCylinderShape, cylinder.clCylinderShape);
		}

		int CylinderCollisionShapeZ_c::Compare(const Key_s &other) const
		{
			return CompareCylinders(clCylinderShape, other);
		}
	}
}
