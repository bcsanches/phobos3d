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
			upGhostObject(new btPairCachingGhostObject()),
			clCollisionTag(collisionTag)
		{
			stepHeight *= Manager_c::GetInstance().GetScale();

			upGhostObject->setCollisionShape(&collisionShape->GetCollisionShape());
			upGhostObject->setCollisionFlags (btCollisionObject::CF_CHARACTER_OBJECT);

			//spCharacterController.reset(PH_NEW btKinematicCharacterController(&*spGhostObject, static_cast<btConvexShape *>(&collisionShape->GetCollisionShape()), stepHeight));
			upCharacterController.reset(PH_NEW KinematicCharacterController_c(&*upGhostObject, static_cast<btConvexShape *>(&collisionShape->GetCollisionShape()), stepHeight));
		}

		GhostCharacterBody_c::~GhostCharacterBody_c()
		{
			this->Unregister();
		}

		void GhostCharacterBody_c::Register()
		{
			Physics::Manager_c &manager = Manager_c::GetInstance();

			//manager->AddCollisionObject(*spGhostObject, btBroadphaseProxy::CharacterFilter|1, btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter);
			manager.AddCollisionObject(*upGhostObject, clCollisionTag);
			manager.AddAction(*upCharacterController);
		}
		
		void GhostCharacterBody_c::Unregister()
		{
			Physics::Manager_c &manager = Manager_c::GetInstance();

			manager.RemoveCollisionObject(*upGhostObject);
			manager.RemoveAction(*upCharacterController);
		}

		void GhostCharacterBody_c::SetVelocityForTimeInterval(const Ogre::Vector3 &velocity, Float_t timeInvertal)
		{
			upCharacterController->setVelocityForTimeInterval(MakeVector3(velocity, Manager_c::GetInstance().GetScale()), timeInvertal);
		}

		Ogre::Vector3 GhostCharacterBody_c::GetPosition() const
		{
			return MakeVector3(upGhostObject->getWorldTransform().getOrigin(), Manager_c::GetInstance().GetPhysicsToGameScale());
		}

		void GhostCharacterBody_c::Teleport(const Ogre::Vector3 &position)
		{
			upCharacterController->warp(MakeVector3(position, Manager_c::GetInstance().GetScale()));
		}
	}
}
