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

#include "PH_CharacterBodyComponent.h"

#include <PH_Transform.h>
#include <PH_TransformProperty.h>

#include "PH_CollisionTag.h"
#include "PH_EntityComponentFactory.h"
#include "PH_EntityKeys.h"
#include "PH_EntityUtils.h"
#include "PH_GameRegisterUtils.h"
#include "PH_GamePhysicsSettings.h"
#include "PH_PhysicsManager.h"
#include "PH_ICharacterBody.h"

namespace Phobos
{
	namespace Physics
	{
		PH_FULL_ENTITY_COMPONENT_CREATOR(PH_CHARACTER_BODY_COMPONENT_NAME, CharacterBodyComponent);
		
		CharacterBodyComponent::CharacterBodyComponent(const String_t &name, Entity &owner):
			EntityComponent(name, owner),			
			m_pprpTransform(NULL),
			m_prpVelocity("velocity"),
			m_prpCharacterPosition("characterPosition")
		{
			owner.AddProperty(m_prpVelocity);
			owner.AddProperty(m_prpCharacterPosition);
		}

		CharacterBodyComponent::~CharacterBodyComponent()
		{			
			Manager::GetInstance().UnregisterCharacterBodyComponent(*this);
		}

		void CharacterBodyComponent::FinishPhysicsFrame()
		{			
			m_prpCharacterPosition.SetVector(m_spCharacterBody->GetPosition());
		}

		void CharacterBodyComponent::PreparePhysicsFrame(Float_t delta)
		{			
			m_spCharacterBody->SetVelocityForTimeInterval(m_prpVelocity.GetVector(), delta);			
		}

		void CharacterBodyComponent::OnLoad(const Register::Table &table)
		{	
			Transform transform;

			EntityLoadTransform(transform, table);						

			Physics::Manager &physicsManager = Manager::GetInstance();

			Float_t height = table.GetFloat("height");
			Float_t radius = table.GetFloat("radius");
			Float_t stepHeight = table.GetFloat("stepHeight");

			Float_t offset = 0;
			table.TryGetFloat(offset, "characterStartHeight");

			Ogre::Vector3 startPos = transform.GetOrigin();
			startPos.y += offset;

			Physics::CollisionTag collisionTag = GamePhysicsSettings::LoadCollisionTag(table);

			m_prpCharacterPosition.SetVector(startPos);
			m_spCharacterBody = physicsManager.CreateCharacterBody(startPos, collisionTag, stepHeight, radius, height);											
		}

		void CharacterBodyComponent::OnLoadFinished()
		{
			Physics::Manager &manager = Manager::GetInstance();
			manager.RegisterCharacterBodyComponent(*this);
			m_spCharacterBody->Register();			

			m_pprpTransform = &this->GetCustomEntityProperty<TransformProperty>(PH_ENTITY_PROP_TRANSFORM);
		}
	}
}
