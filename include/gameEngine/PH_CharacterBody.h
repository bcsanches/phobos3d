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

#include <boost/scoped_ptr.hpp>

#include <OgrePrerequisites.h>

#include <PH_Types.h>

#include "PH_CollisionShapeFwd.h"

namespace Phobos
{
	namespace Physics
	{
		class CharacterBody_c
		{
			public:
				CharacterBody_c(Float_t stepHeight, CollisionShapePtr_t collisionShape);
				~CharacterBody_c();

				void SetWalkDirection(const Ogre::Vector3 &walkDirection);

				void Register();
				void Unregister();

			private:
				boost::scoped_ptr<btPairCachingGhostObject> spGhostObject;
				boost::scoped_ptr<btKinematicCharacterController> spCharacterController;

				CollisionShapePtr_t	spCollisionShape;
		};
	}
}