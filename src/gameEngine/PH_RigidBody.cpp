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
		RigidBody_c::RigidBody_c(RigidBodyTypes_e type, const btRigidBody::btRigidBodyConstructionInfo &info, btDefaultMotionState *motionState, CollisionShapePtr_t shape, const CollisionTag_c &collisionTag):
			upRigidBody(new btRigidBody(info)),
			spCollisionShape(shape),
			upMotionState(motionState),
			clCollisionTag(collisionTag)
		{
			if(type == RBT_KINEMATIC)
			{
				upRigidBody->setCollisionFlags( upRigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT );
				upRigidBody->setActivationState(DISABLE_DEACTIVATION);
			}
			else if(type == RBT_STATIC)
			{
				upRigidBody->setCollisionFlags( upRigidBody->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT );
			}
		}

		RigidBody_c::~RigidBody_c()
		{
			if(upRigidBody->isInWorld())
				this->Unregister();
		}

		void RigidBody_c::Register()
		{
			Manager_c::GetInstance().RegisterRigidBody(*upRigidBody, clCollisionTag);
		}
		
		void RigidBody_c::Unregister()
		{
			Manager_c::GetInstance().UnregisterRigidBody(*upRigidBody);
		}

		Transform_c RigidBody_c::GetTransform() const
		{
			btTransform bodyTransform;
			upMotionState->getWorldTransform(bodyTransform);

			return MakeTransform(bodyTransform, Manager_c::GetInstance().GetPhysicsToGameScale());
		}

		void RigidBody_c::SetKinematicTransform(const btTransform &transform)
		{
			upMotionState->setWorldTransform(transform);
		}
	}
}
