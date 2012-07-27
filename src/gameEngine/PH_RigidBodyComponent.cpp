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

#include "PH_EntityComponentFactory.h"
#include "PH_EntityKeys.h"
#include "PH_EntityUtils.h"
#include "PH_GameDictionaryUtils.h"
#include "PH_PhysicsManager.h"
#include "PH_RigidBody.h"

namespace Phobos
{
	namespace Physics
	{
		PH_FULL_ENTITY_COMPONENT_CREATOR(PH_RIGID_BODY_COMPONENT_NAME, RigidBodyComponent_c);

		struct ShapeTypeName_s
		{
			const char *pstrzName;
			PhysicsManager_c::CollisionShapeTypes_e eValue;
		};

		static const ShapeTypeName_s stShapeTypeNameTable_gl[] = 
		{
			{"box", PhysicsManager_c::CST_BOX},
			{"sphere", PhysicsManager_c::CST_SPHERE},
			{"mesh", PhysicsManager_c::CST_MESH},

			{NULL, PhysicsManager_c::CST_BOX}
		};

		RigidBodyComponent_c::RigidBodyComponent_c(const String_c &name, Entity_c &owner):
			EntityComponent_c(name, owner),			
			pprpTransform(NULL)
		{
			//empty
		}

		RigidBodyComponent_c::~RigidBodyComponent_c()
		{			
			PhysicsManager_c::GetInstance()->UnregisterRigidBodyComponent(*this);
		}

		void RigidBodyComponent_c::SaveTransform()
		{			
			clPreviousTransform = spRigidBody->GetTransform();
		}

		void RigidBodyComponent_c::UpdateTransform(Float_t delta)
		{			
			pprpTransform->SetTransform(Transform_c::Interpolate(clPreviousTransform, spRigidBody->GetTransform(), delta));
		}

		void RigidBodyComponent_c::OnLoad(const Dictionary_c &dictionary)
		{	
			Transform_c transform;

			EntityLoadTransform(transform, dictionary);			

			Enum_c<PhysicsManager_c::CollisionShapeTypes_e,  ShapeTypeName_s> enumMap(stShapeTypeNameTable_gl);

			PhysicsManager_c::CollisionShapeTypes_e type;
			if(!enumMap.TryGetValue(type, dictionary.GetString("colliderType")))
			{
				std::stringstream stream;
				stream << "Invalid colliderType " << dictionary.GetString("colliderType") << " for entity " << this->GetName();
				PH_RAISE(INVALID_PARAMETER_EXCEPTION, "RigidBodyComponent_c::OnLoad", stream.str());
			}

			PhysicsManagerPtr_t physicsManager = PhysicsManager_c::GetInstance();

			PH_ASSERT(!spRigidBody);			
			
			Float_t mass = dictionary.GetFloat("mass");

			switch(type)
			{
				case PhysicsManager_c::CST_BOX:
					{
						Ogre::Vector3 boxDimensions = boxDimensions = DictionaryGetVector3(dictionary, "boxDimensions");						
						spRigidBody = physicsManager->CreateBoxRigidBody(RBT_DYNAMIC, transform, mass, boxDimensions.x, boxDimensions.y, boxDimensions.z );			
					}
					break;

				default:
					PH_RAISE(INVALID_PARAMETER_EXCEPTION, "RigidBodyComponent_c::OnLoad", "Shape not supported");
					break;
			}									
		}

		void RigidBodyComponent_c::OnLoadFinished()
		{
			PhysicsManagerPtr_t manager = PhysicsManager_c::GetInstance();
			manager->RegisterRigidBodyComponent(*this);
			spRigidBody->Register();			

			pprpTransform = &this->GetCustomEntityProperty<TransformProperty_c>(PH_ENTITY_PROP_TRANSFORM);

			this->SaveTransform();
		}
	}
}
