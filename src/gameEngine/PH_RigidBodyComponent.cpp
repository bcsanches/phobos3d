/*
Phobos 3d
October 2011
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

#include "PH_RigidBodyComponent.h"

#include <PH_Enum.h>
#include <PH_Transform.h>
#include <PH_TransformProperty.h>

#include "PH_CollisionTag.h"
#include "PH_EntityComponentFactory.h"
#include "PH_EntityKeys.h"
#include "PH_EntityUtils.h"
#include "PH_GameRegisterUtils.h"
#include "PH_GamePhysicsSettings.h"
#include "PH_PhysicsManager.h"
#include "PH_PhysicsUtils.h"
#include "PH_RigidBody.h"

namespace Phobos
{
	namespace Physics
	{
		PH_FULL_ENTITY_COMPONENT_CREATOR(PH_RIGID_BODY_COMPONENT_NAME, RigidBodyComponent_c);		

		RigidBodyComponent_c::RigidBodyComponent_c(const String_t &name, Entity_c &owner):
			EntityComponent_c(name, owner),			
			pprpTransform(NULL)
		{
			//empty
		}

		RigidBodyComponent_c::~RigidBodyComponent_c()
		{			
			Manager_c::GetInstance().UnregisterRigidBodyComponent(*this);
		}

		void RigidBodyComponent_c::SaveTransform()
		{			
			clPreviousTransform = spRigidBody->GetTransform();
		}

		void RigidBodyComponent_c::UpdateTransform(Float_t delta)
		{			
			pprpTransform->SetTransform(Transform_c::Interpolate(clPreviousTransform, spRigidBody->GetTransform(), delta));
		}

		void RigidBodyComponent_c::OnLoad(const Register::Table_c &table)
		{	
			Transform_c transform;

			EntityLoadTransform(transform, table);						

			Physics::Manager_c &physicsManager = Manager_c::GetInstance();

			Physics::CollisionTag_c collisionTag = GamePhysicsSettings_c::LoadCollisionTag(table);

			PH_ASSERT(!spRigidBody);			
			
			Float_t mass = table.GetFloat("mass");

			spRigidBody = physicsManager.CreateRigidBody(RBT_DYNAMIC, transform, mass, collisionTag, Physics::Utils::CreateCollisionShape(table, Ogre::Vector3(1, 1, 1)));
		}

		void RigidBodyComponent_c::OnLoadFinished()
		{
			Physics::Manager_c &manager = Manager_c::GetInstance();
			manager.RegisterRigidBodyComponent(*this);
			spRigidBody->Register();			

			pprpTransform = &this->GetCustomEntityProperty<TransformProperty_c>(PH_ENTITY_PROP_TRANSFORM);

			this->SaveTransform();
		}
	}
}
