#include "PH_GhostCharacterBody.h"

#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletCollision/CollisionShapes/btConvexShape.h>
//#include <BulletDynamics/Character/btKinematicCharacterController.h>

#include "PH_CollisionShape.h"

#include "PH_KinematicCharacterController.h"
#include "PH_PhysicsManager.h"
#include "PH_PhysicsUtils.h"

namespace Phobos
{
	namespace Physics
	{
		GhostCharacterBody_c::GhostCharacterBody_c(Float_t stepHeight, CollisionShapePtr_t collisionShape):
			spCollisionShape(collisionShape),
			spGhostObject(new btPairCachingGhostObject())
		{
			stepHeight *= PhysicsManager_c::GetInstance()->GetScale();

			spGhostObject->setCollisionShape(&collisionShape->GetCollisionShape());
			spGhostObject->setCollisionFlags (btCollisionObject::CF_CHARACTER_OBJECT);

			//spCharacterController.reset(new btKinematicCharacterController(&*spGhostObject, static_cast<btConvexShape *>(&collisionShape->GetCollisionShape()), stepHeight));
			spCharacterController.reset(new KinematicCharacterController_c(&*spGhostObject, static_cast<btConvexShape *>(&collisionShape->GetCollisionShape()), stepHeight));
		}

		GhostCharacterBody_c::~GhostCharacterBody_c()
		{
			this->Unregister();
		}

		void GhostCharacterBody_c::Register()
		{
			PhysicsManagerPtr_t manager = PhysicsManager_c::GetInstance();

			manager->AddCollisionObject(*spGhostObject, btBroadphaseProxy::CharacterFilter|1, btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter);
			manager->AddAction(*spCharacterController);
		}
		
		void GhostCharacterBody_c::Unregister()
		{
			PhysicsManagerPtr_t manager = PhysicsManager_c::GetInstance();

			manager->RemoveCollisionObject(*spGhostObject);
			manager->RemoveAction(*spCharacterController);
		}

		void GhostCharacterBody_c::SetWalkDirection(const Ogre::Vector3 &walkDirection)
		{
			spCharacterController->setWalkDirection(MakeVector3(walkDirection, PhysicsManager_c::GetInstance()->GetScale()));
		}

		void GhostCharacterBody_c::SetVelocityForTimeInterval(const Ogre::Vector3 &velocity, Float_t timeInvertal)
		{
			spCharacterController->setVelocityForTimeInterval(MakeVector3(velocity, PhysicsManager_c::GetInstance()->GetScale()), timeInvertal);
		}

		Ogre::Vector3 GhostCharacterBody_c::GetPosition() const
		{
			return MakeVector3(spGhostObject->getWorldTransform().getOrigin(), PhysicsManager_c::GetInstance()->GetPhysicsToGameScale());
		}

		void GhostCharacterBody_c::Teleport(const Ogre::Vector3 &position)
		{
			spCharacterController->warp(MakeVector3(position, PhysicsManager_c::GetInstance()->GetScale()));
		}
	}
}
