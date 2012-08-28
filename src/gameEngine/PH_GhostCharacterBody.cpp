#include "PH_GhostCharacterBody.h"

#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletCollision/CollisionShapes/btConvexShape.h>
//#include <BulletDynamics/Character/btKinematicCharacterController.h>

#include "PH_CollisionShape.h"

#include "PH_KinematicCharacterController.h"
#include "PH_PhysicsManager.h"
#include "PH_PhysicsConv.h"

namespace Phobos
{
	namespace Physics
	{
		GhostCharacterBody_c::GhostCharacterBody_c(Float_t stepHeight, CollisionShapePtr_t collisionShape, const CollisionTag_c &collisionTag):
			spCollisionShape(collisionShape),
			spGhostObject(new btPairCachingGhostObject()),
			clCollisionTag(collisionTag)
		{
			stepHeight *= Manager_c::GetInstance()->GetScale();

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
			Physics::ManagerPtr_t manager = Manager_c::GetInstance();

			//manager->AddCollisionObject(*spGhostObject, btBroadphaseProxy::CharacterFilter|1, btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter);
			manager->AddCollisionObject(*spGhostObject, clCollisionTag);
			manager->AddAction(*spCharacterController);
		}
		
		void GhostCharacterBody_c::Unregister()
		{
			Physics::ManagerPtr_t manager = Manager_c::GetInstance();

			manager->RemoveCollisionObject(*spGhostObject);
			manager->RemoveAction(*spCharacterController);
		}

		void GhostCharacterBody_c::SetVelocityForTimeInterval(const Ogre::Vector3 &velocity, Float_t timeInvertal)
		{
			spCharacterController->setVelocityForTimeInterval(MakeVector3(velocity, Manager_c::GetInstance()->GetScale()), timeInvertal);
		}

		Ogre::Vector3 GhostCharacterBody_c::GetPosition() const
		{
			return MakeVector3(spGhostObject->getWorldTransform().getOrigin(), Manager_c::GetInstance()->GetPhysicsToGameScale());
		}

		void GhostCharacterBody_c::Teleport(const Ogre::Vector3 &position)
		{
			spCharacterController->warp(MakeVector3(position, Manager_c::GetInstance()->GetScale()));
		}
	}
}
