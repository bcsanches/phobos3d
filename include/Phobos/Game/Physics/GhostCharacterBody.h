/*
Phobos 3d
July 2012
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

#ifndef PH_GHOST_CHARACTER_BODY_H
#define PH_GHOST_CHARACTER_BODY_H

class btPairCachingGhostObject;

#include <OgrePrerequisites.h>

#include <Phobos/Types.h>

#include "Phobos/Game/Physics/ICharacterBody.h"
#include "Phobos/Game/Physics/CollisionShapeFwd.h"
#include "Phobos/Game/Physics/CollisionTag.h"
#include "Phobos/Game/GameAPI.h"

namespace Phobos
{
	namespace Game
	{
		namespace Physics
		{
			class KinematicCharacterController;

			class PH_GAME_API GhostCharacterBody: public ICharacterBody
			{
				public:
					GhostCharacterBody(Float_t stepHeight, CollisionShapePtr_t collisionShape, const CollisionTag &collisionTag);
					virtual ~GhostCharacterBody();
				
					virtual void SetVelocityForTimeInterval(const Ogre::Vector3 &velocity, Float_t timeInvertal);

					virtual Ogre::Vector3 GetPosition() const;

					virtual void Register();
					virtual void Unregister();

					virtual void Teleport(const Ogre::Vector3 &position);

				private:
					std::unique_ptr<btPairCachingGhostObject> m_upGhostObject;				
					std::unique_ptr<KinematicCharacterController> m_upCharacterController;

					CollisionShapePtr_t	m_spCollisionShape;

					CollisionTag m_clCollisionTag;
			};
		}
	}
}


#endif