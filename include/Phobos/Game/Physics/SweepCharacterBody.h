/*
Phobos 3d
July 2012
Copyright (c) 2005-2011 Bruno Sanches  http://code.google.com/p/phobos3d

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

class btPairCachingGhostObject;
class btKinematicCharacterController;

#include <OgrePrerequisites.h>

#include <BulletDynamics/Dynamics/btActionInterface.h>
#include <LinearMath/btVector3.h>

#include <Phobos/Types.h>

#include "Phobos/Game/Physics/ICharacterBody.h"
#include "Phobos/Game/GameAPI.h"
#include "Phobos/Game/Physics/RigidBodyFwd.h"

namespace Phobos
{
	namespace Game
	{
		namespace Physics
		{
			class PH_GAME_API SweepCharacterBody: public ICharacterBody,  private btActionInterface
			{
				public:
					SweepCharacterBody(RigidBodyPtr_t rigidBody, Float_t stepHeight);
					~SweepCharacterBody();				

					virtual void SetVelocityForTimeInterval(const Ogre::Vector3 &velocity, Float_t timeInvertal);

					virtual Ogre::Vector3 GetPosition() const;

					virtual void Register();
					virtual void Unregister();

					void Teleport(const Ogre::Vector3 &position);

				private:
					//For bullet
					virtual void updateAction( btCollisionWorld* collisionWorld, btScalar deltaTimeStep);

					virtual void debugDraw(btIDebugDraw* debugDrawer);

					bool GroundTrace(const btVector3 &position);

					void Move(const btTransform &fromTransform, const btVector3 &linearVel, btTransform &toTransform, Float_t timeStep);

				private:
					RigidBodyPtr_t m_spRigidBody;

					Float_t m_fpStepHeight;

					btVector3 m_v3WalkDirection;

					btVector3 m_v3GroundNormal;

					bool m_fGroundPlane;
					bool m_fWalking;
			};
		}
	}
}
