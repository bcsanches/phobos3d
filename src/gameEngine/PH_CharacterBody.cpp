#include "PH_CharacterBody.h"

#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletCollision/CollisionShapes/btConvexShape.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>

#include "PH_CollisionShape.h"

#include "PH_PhysicsManager.h"
#include "PH_PhysicsUtils.h"

namespace Phobos
{
	namespace Physics
	{
		CharacterBody_c::CharacterBody_c(Float_t stepHeight, CollisionShapePtr_t collisionShape):
			spCollisionShape(collisionShape),
			spGhostObject(new btPairCachingGhostObject())
		{
			stepHeight *= PhysicsManager_c::GetInstance()->GetScale();

			spGhostObject->setCollisionShape(&collisionShape->GetCollisionShape());
			spGhostObject->setCollisionFlags (btCollisionObject::CF_CHARACTER_OBJECT);

			spCharacterController.reset(new btKinematicCharacterController(&*spGhostObject, static_cast<btConvexShape *>(&collisionShape->GetCollisionShape()), stepHeight));
		}

		CharacterBody_c::~CharacterBody_c()
		{
			this->Unregister();
		}

		void CharacterBody_c::Register()
		{
			PhysicsManagerPtr_t manager = PhysicsManager_c::GetInstance();

			manager->AddCollisionObject(*spGhostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter);
			manager->AddAction(*spCharacterController);
		}
		
		void CharacterBody_c::Unregister()
		{
			PhysicsManagerPtr_t manager = PhysicsManager_c::GetInstance();

			manager->RemoveCollisionObject(*spGhostObject);
			manager->RemoveAction(*spCharacterController);
		}

		void CharacterBody_c::SetWalkDirection(const Ogre::Vector3 &walkDirection)
		{
			spCharacterController->setWalkDirection(MakeVector3(walkDirection, PhysicsManager_c::GetInstance()->GetScale()));
		}
	}
}
