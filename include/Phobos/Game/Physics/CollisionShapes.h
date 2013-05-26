#ifndef PH_COLLISION_SHAPES_H
#define PH_COLLISION_SHAPES_H

#include "Phobos/Game/Physics/CollisionShape.h"

#include <BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <BulletCollision/CollisionShapes/btCapsuleShape.h>
#include <BulletCollision/CollisionShapes/btCylinderShape.h>
#include <OgrePrerequisites.h>

#include "Phobos/Game/Physics/CollisionMeshFwd.h"

namespace Phobos
{
	namespace Game
	{
		namespace Physics
		{
			class BoxCollisionShape: public CollisionShape
			{
				public:
					BoxCollisionShape(const Ogre::Vector3 &dimensions, Float_t physicsScale);

					virtual btCollisionShape &GetCollisionShape();

				protected:
					virtual int Compare(const CollisionShape &other) const;
					virtual int Compare(const Key_s &other) const;				

				private:
					btBoxShape m_clBoxShape;
			};

			class CapsuleCollisionShape: public CollisionShape
			{
				public:
					CapsuleCollisionShape(Float_t radius, Float_t height, Float_t physicsScale);

					virtual btCollisionShape &GetCollisionShape();

				protected:
					virtual int Compare(const CollisionShape &other) const;
					virtual int Compare(const Key_s &other) const;				

				private:
					btCapsuleShape m_clCapsuleShape;
			};
		

			class ScaledMeshCollissionShape: public CollisionShape
			{
				public:
					ScaledMeshCollissionShape(CollisionMeshPtr_t collisionMesh, const Ogre::Vector3 &localScale, Float_t physicsScale);

					virtual btCollisionShape &GetCollisionShape();			

				private:
					virtual int Compare(const CollisionShape &other) const;
					virtual int Compare(const Key_s &other) const;

				private:	
					//This is used only to keep reference alive
					CollisionMeshPtr_t				m_spOriginalMesh;

					btScaledBvhTriangleMeshShape	m_clScaleTriangleMeshShape;
			};

			class CylinderCollisionShapeX: public CollisionShape
			{
				public:
					CylinderCollisionShapeX(Float_t radius, Float_t height, Float_t physicsScale);

					virtual btCollisionShape &GetCollisionShape();

				protected:
					virtual int Compare(const CollisionShape &other) const;
					virtual int Compare(const Key_s &other) const;

				private:
					btCylinderShapeX m_clCylinderShape;
			};

			class CylinderCollisionShapeY: public CollisionShape
			{
				public:
					CylinderCollisionShapeY(Float_t radius, Float_t height, Float_t physicsScale);

					virtual btCollisionShape &GetCollisionShape();

				protected:
					virtual int Compare(const CollisionShape &other) const;
					virtual int Compare(const Key_s &other) const;				

				private:
					btCylinderShape m_clCylinderShape;

			};

			class CylinderCollisionShapeZ: public CollisionShape
			{
				public:
					CylinderCollisionShapeZ(Float_t radius, Float_t height, Float_t physicsScale);

					virtual btCollisionShape &GetCollisionShape();

				protected:
					virtual int Compare(const CollisionShape &other) const;
					virtual int Compare(const Key_s &other) const;				

				private:
					btCylinderShapeZ m_clCylinderShape;

			};
		}
	}
}

#endif