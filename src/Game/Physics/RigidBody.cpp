/*
Phobos 3d
June 2012
Copyright (c) 2005-2012 Bruno Sanches  http://code.google.com/p/phobos3d

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#include "Phobos/Game/Physics/RigidBody.h"

#include "Phobos/Game/Physics/Manager.h"
#include "Phobos/Game/Physics/Conv.h"

namespace Phobos
{	
	namespace Game
	{
		namespace Physics
		{
			RigidBody::RigidBody():
				m_clCollisionTag(0, 0)
			{
				//empty
			}

			RigidBody::RigidBody(RigidBodyTypes_e type, const btRigidBody::btRigidBodyConstructionInfo &info, std::unique_ptr<btDefaultMotionState> &&motionState, CollisionShapePtr_t shape, const CollisionTag &collisionTag):
				m_upRigidBody(new btRigidBody(info)),
				m_spCollisionShape(shape),
				m_upMotionState(std::move(motionState)),
				m_clCollisionTag(collisionTag)
			{
				if(type == RBT_KINEMATIC)
				{
					m_upRigidBody->setCollisionFlags( m_upRigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT );
					m_upRigidBody->setActivationState(DISABLE_DEACTIVATION);
				}
				else if(type == RBT_STATIC)
				{
					m_upRigidBody->setCollisionFlags( m_upRigidBody->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT );
				}
			}

			RigidBody::RigidBody(RigidBody &&other):
				m_upRigidBody(std::move(other.m_upRigidBody)),
				m_spCollisionShape(std::move(other.m_spCollisionShape)),
				m_upMotionState(std::move(other.m_upMotionState)),				
				m_clCollisionTag(std::move(other.m_clCollisionTag))
			{
				//empty
			}

			RigidBody::~RigidBody()
			{
				if(m_upRigidBody && m_upRigidBody->isInWorld())
					this->Unregister();
			}

			RigidBody &RigidBody::operator=(RigidBody &&other)
			{
				m_upRigidBody = std::move(other.m_upRigidBody);
				m_spCollisionShape = std::move(other.m_spCollisionShape);
				m_upMotionState = std::move(other.m_upMotionState);
				m_clCollisionTag = std::move(other.m_clCollisionTag);

				return *this;
			}

			void RigidBody::Register()
			{
				PH_ASSERT(m_upRigidBody.get());

				Manager::GetInstance().RegisterRigidBody(*m_upRigidBody, m_clCollisionTag);
			}
		
			void RigidBody::Unregister()
			{
				PH_ASSERT(m_upRigidBody.get());

				Manager::GetInstance().UnregisterRigidBody(*m_upRigidBody);
			}

			Engine::Math::Transform RigidBody::GetTransform() const
			{
				btTransform bodyTransform;
				m_upMotionState->getWorldTransform(bodyTransform);

				return MakeTransform(bodyTransform, Manager::GetInstance().GetPhysicsToGameScale());
			}

			void RigidBody::SetKinematicTransform(const btTransform &transform)
			{
				m_upMotionState->setWorldTransform(transform);
			}

			void RigidBody::Warp(const Engine::Math::Transform &transform)
			{
				auto bodyTransform = MakeTransform(transform, Manager::GetInstance().GetPhysicsToGameScale());

				bool registered = m_upRigidBody->isInWorld();
				if (registered)
					this->Unregister();

				m_upMotionState->setWorldTransform(bodyTransform);
				m_upRigidBody->setWorldTransform(bodyTransform);

				if (registered)
					this->Register();
			}
		}
	}
}
