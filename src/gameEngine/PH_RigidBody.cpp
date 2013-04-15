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

#include "PH_RigidBody.h"

#include "PH_PhysicsManager.h"
#include "PH_PhysicsConv.h"

namespace Phobos
{	
	namespace Physics
	{
		RigidBody::RigidBody(RigidBodyTypes_e type, const btRigidBody::btRigidBodyConstructionInfo &info, btDefaultMotionState *motionState, CollisionShapePtr_t shape, const CollisionTag &collisionTag):
			m_upRigidBody(new btRigidBody(info)),
			m_spCollisionShape(shape),
			m_upMotionState(motionState),
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

		RigidBody::~RigidBody()
		{
			if(m_upRigidBody->isInWorld())
				this->Unregister();
		}

		void RigidBody::Register()
		{
			Manager::GetInstance().RegisterRigidBody(*m_upRigidBody, m_clCollisionTag);
		}
		
		void RigidBody::Unregister()
		{
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
	}
}
