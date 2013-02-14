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

#ifndef PH_RIGID_BODY_H
#define PH_RIGID_BODY_H

#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <LinearMath/btDefaultMotionState.h>

#include <Phobos/Types.h>

#include "PH_CollisionShapeFwd.h"
#include "PH_CollisionTag.h"
#include "PH_RigidBodyFwd.h"

namespace Phobos
{
	class Transform;

	namespace Physics
	{		
		class RigidBody
		{
			public:
				RigidBody(RigidBodyTypes_e type, const btRigidBody::btRigidBodyConstructionInfo &info, btDefaultMotionState *motionState, CollisionShapePtr_t shape, const CollisionTag &collisionTag);
				~RigidBody();

				void Register();
				void Unregister();

				Transform GetTransform() const;

				inline const btRigidBody &GetRigidBody() const;

				void SetKinematicTransform(const btTransform &transform);

			private:
				//Unless we force this class to be allocated on 16 bytes aligned memory, we cannot create RigidBody without using new :(				
				std::unique_ptr<btRigidBody>			m_upRigidBody;
				std::unique_ptr<btDefaultMotionState>	m_upMotionState;
				CollisionShapePtr_t						m_spCollisionShape;

				CollisionTag							m_clCollisionTag;

		};

		inline const btRigidBody &RigidBody::GetRigidBody() const
		{
			return *m_upRigidBody;
		}
	}
}

#endif
