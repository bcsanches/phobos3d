#include "Phobos/Game/Physics/GhostCharacterBody.h"

#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletCollision/CollisionShapes/btConvexShape.h>
//#include <BulletDynamics/Character/btKinematicCharacterController.h>

#include "Phobos/Game/Physics/CollisionShape.h"

#include "Phobos/Game/Physics/KinematicCharacterController.h"
#include "Phobos/Game/Physics/Manager.h"
#include "Phobos/Game/Physics/Conv.h"

namespace Phobos
{
	namespace Game
	{
		namespace Physics
		{
			GhostCharacterBody::GhostCharacterBody(Float_t stepHeight, CollisionShapePtr_t collisionShape, const CollisionTag &collisionTag):
				m_spCollisionShape(collisionShape),
				m_upGhostObject(new btPairCachingGhostObject()),
				m_clCollisionTag(collisionTag)
			{
				stepHeight *= Manager::GetInstance().GetScale();

				m_upGhostObject->setCollisionShape(&collisionShape->GetCollisionShape());
				m_upGhostObject->setCollisionFlags (btCollisionObject::CF_CHARACTER_OBJECT);

				//spCharacterController.reset(PH_NEW btKinematicCharacterController(&*spGhostObject, static_cast<btConvexShape *>(&collisionShape->GetCollisionShape()), stepHeight));
				m_upCharacterController.reset(PH_NEW KinematicCharacterController(&*m_upGhostObject, static_cast<btConvexShape *>(&collisionShape->GetCollisionShape()), stepHeight));
			}

			GhostCharacterBody::~GhostCharacterBody()
			{
				this->Unregister();
			}

			void GhostCharacterBody::Register()
			{
				Physics::Manager &manager = Manager::GetInstance();

				//manager->AddCollisionObject(*spGhostObject, btBroadphaseProxy::CharacterFilter|1, btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter);
				manager.AddCollisionObject(*m_upGhostObject, m_clCollisionTag);
				manager.AddAction(*m_upCharacterController);
			}
		
			void GhostCharacterBody::Unregister()
			{
				Physics::Manager &manager = Manager::GetInstance();

				manager.RemoveCollisionObject(*m_upGhostObject);
				manager.RemoveAction(*m_upCharacterController);
			}

			void GhostCharacterBody::SetVelocityForTimeInterval(const Ogre::Vector3 &velocity, Float_t timeInvertal)
			{
				m_upCharacterController->setVelocityForTimeInterval(MakeVector3(velocity, Manager::GetInstance().GetScale()), timeInvertal);
			}

			Ogre::Vector3 GhostCharacterBody::GetPosition() const
			{
				return MakeVector3(m_upGhostObject->getWorldTransform().getOrigin(), Manager::GetInstance().GetPhysicsToGameScale());
			}

			void GhostCharacterBody::Teleport(const Ogre::Vector3 &position)
			{
				m_upCharacterController->warp(MakeVector3(position, Manager::GetInstance().GetScale()));
			}
		}
	}
}
