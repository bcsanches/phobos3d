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

#include "PH_EntityComponentFactory.h"
#include "PH_EntityKeys.h"
#include "PH_EntityUtils.h"
#include "PH_GameDictionaryUtils.h"
#include "PH_PhysicsManager.h"
#include "PH_ICharacterBody.h"

namespace Phobos
{
	namespace Physics
	{
		PH_FULL_ENTITY_COMPONENT_CREATOR(PH_CHARACTER_BODY_COMPONENT_NAME, CharacterBodyComponent_c);
		
		CharacterBodyComponent_c::CharacterBodyComponent_c(const String_c &name, Entity_c &owner):
			EntityComponent_c(name, owner),			
			pprpTransform(NULL),
			prpVelocity("velocity"),
			prpCharacterPosition("characterPosition")
		{
			owner.AddProperty(prpVelocity);
			owner.AddProperty(prpCharacterPosition);
		}

		CharacterBodyComponent_c::~CharacterBodyComponent_c()
		{			
			PhysicsManager_c::GetInstance()->UnregisterCharacterBodyComponent(*this);
		}

		void CharacterBodyComponent_c::FinishPhysicsFrame()
		{			
			prpCharacterPosition.SetVector(spCharacterBody->GetPosition());
		}

		void CharacterBodyComponent_c::PreparePhysicsFrame(Float_t delta)
		{			
			spCharacterBody->SetVelocityForTimeInterval(prpVelocity.GetVector(), delta);			
		}

		void CharacterBodyComponent_c::OnLoad(const Dictionary_c &dictionary)
		{	
			Transform_c transform;

			EntityLoadTransform(transform, dictionary);						

			PhysicsManagerPtr_t physicsManager = PhysicsManager_c::GetInstance();

			Float_t height = dictionary.GetFloat("height");
			Float_t radius = dictionary.GetFloat("radius");
			Float_t stepHeight = dictionary.GetFloat("stepHeight");

			Float_t offset = 0;
			dictionary.TryGetFloat(offset, "characterStartHeight");

			Ogre::Vector3 startPos = transform.GetOrigin();
			startPos.y += offset;

			prpCharacterPosition.SetVector(startPos);
			spCharacterBody = physicsManager->CreateCharacterBody(startPos, stepHeight, radius, height);

			PH_ASSERT(!spRigidBody);													
		}

		void CharacterBodyComponent_c::OnLoadFinished()
		{
			PhysicsManagerPtr_t manager = PhysicsManager_c::GetInstance();
			manager->RegisterCharacterBodyComponent(*this);
			spCharacterBody->Register();			

			pprpTransform = &this->GetCustomEntityProperty<TransformProperty_c>(PH_ENTITY_PROP_TRANSFORM);
		}
	}
}
