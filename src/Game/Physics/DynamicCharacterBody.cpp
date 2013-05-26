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


#include "Phobos/Game/Physics/DynamicCharacterBody.h"

#include <OgreVector3.h>

#include <Phobos/OgreEngine/Math/Transform.h>

#include "Phobos/Game/Physics/RigidBody.h"

namespace Phobos
{
	namespace Game
	{
		namespace Physics
		{	
			DynamicCharacterBody::DynamicCharacterBody(RigidBodyPtr_t rigidBody):
				m_spRigidBody(rigidBody)
			{
				//empty
			}

			DynamicCharacterBody::~DynamicCharacterBody()
			{
				//empty
			}

			void DynamicCharacterBody::SetVelocityForTimeInterval(const Ogre::Vector3 &velocity, Float_t timeInvertal)
			{

			}

			Ogre::Vector3 DynamicCharacterBody::GetPosition() const
			{
				return m_spRigidBody->GetTransform().GetOrigin();
			}


			void DynamicCharacterBody::Register()
			{
				m_spRigidBody->Register();
			}

			void DynamicCharacterBody::Unregister()
			{
				m_spRigidBody->Unregister();
			}

			void DynamicCharacterBody::Teleport(const Ogre::Vector3 &position)
			{

			}			
		}
	}
}
