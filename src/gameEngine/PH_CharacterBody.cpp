#include "PH_CharacterBody.h"

#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletCollision/CollisionShapes/btConvexShape.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>

#include "PH_CollisionShape.h"

namespace Phobos
{
	namespace Physics
	{
		CharacterBody_c::CharacterBody_c(Float_t stepHeight, CollisionShapePtr_t collisionShape):
			spCollisionShape(collisionShape),
			spGhostObject(new btPairCachingGhostObject())
		{
			spGhostObject->setCollisionShape(&collisionShape->GetCollisionShape());
			spGhostObject->setCollisionFlags (btCollisionObject::CF_CHARACTER_OBJECT);

			spCharacterController.reset(new btKinematicCharacterController(&*spGhostObject, static_cast<btConvexShape *>(&collisionShape->GetCollisionShape()), stepHeight));
		}
	}
}
