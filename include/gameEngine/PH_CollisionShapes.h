#ifndef PH_COLLISION_SHAPES_H
#define PH_COLLISION_SHAPES_H

#include "PH_CollisionShape.h"

#include <BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <BulletCollision/CollisionShapes/btCapsuleShape.h>
#include <OgrePrerequisites.h>

#include "PH_CollisionMeshFwd.h"

namespace Phobos
{
	namespace Physics
	{
		class BoxCollisionShape_c: public CollisionShape_c
		{
			public:
				BoxCollisionShape_c(const Ogre::Vector3 &dimensions, Float_t physicsScale);

				virtual btCollisionShape &GetCollisionShape();

			protected:
				virtual int Compare(const CollisionShape_c &other) const;
				virtual int Compare(const Key_s &other) const;				

			private:
				btBoxShape clBoxShape;
		};

		class CapsuleCollisionShape_c: public CollisionShape_c
		{
			public:
				CapsuleCollisionShape_c(Float_t radius, Float_t height, Float_t physicsScale);

				virtual btCollisionShape &GetCollisionShape();

			protected:
				virtual int Compare(const CollisionShape_c &other) const;
				virtual int Compare(const Key_s &other) const;				

			private:
				btCapsuleShape clCapsuleShape;
		};
		

		class ScaledMeshCollissionShape_c: public CollisionShape_c
		{
			public:
				ScaledMeshCollissionShape_c(CollisionMeshPtr_t collisionMesh, const Ogre::Vector3 &localScale, Float_t physicsScale);

				virtual btCollisionShape &GetCollisionShape();			

			private:
				virtual int Compare(const CollisionShape_c &other) const;
				virtual int Compare(const Key_s &other) const;

			private:	
				//This is used only to keep reference alive
				CollisionMeshPtr_t				spOriginalMesh;

				btScaledBvhTriangleMeshShape	clScaleTriangleMeshShape;
		};
	}
}

#endif