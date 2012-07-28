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


#include "PH_DynamicCharacterBody.h"

#include <OgreVector3.h>

#include <PH_Transform.h>

#include "PH_RigidBody.h"

namespace Phobos
{
	namespace Physics
	{	
		DynamicCharacterBody_c::DynamicCharacterBody_c(RigidBodyPtr_t rigidBody):
			spRigidBody(rigidBody)
		{
			//empty
		}

		DynamicCharacterBody_c::~DynamicCharacterBody_c()
		{
			//empty
		}

		void DynamicCharacterBody_c::SetVelocityForTimeInterval(const Ogre::Vector3 &velocity, Float_t timeInvertal)
		{

		}

		Ogre::Vector3 DynamicCharacterBody_c::GetPosition() const
		{
			return spRigidBody->GetTransform().GetOrigin();
		}


		void DynamicCharacterBody_c::Register()
		{
			spRigidBody->Register();
		}

		void DynamicCharacterBody_c::Unregister()
		{
			spRigidBody->Unregister();
		}

		void DynamicCharacterBody_c::Teleport(const Ogre::Vector3 &position)
		{

		}			
	}
}
