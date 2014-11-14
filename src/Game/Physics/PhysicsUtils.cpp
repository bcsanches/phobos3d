/*
Phobos 3d
August 2012
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

#include "Phobos/Game/Physics/PhysicsUtils.h"

#include <Phobos/Register/Table.h>

#include <Phobos/Enum.h>
#include <Phobos/Exception.h>

#include "Phobos/Game/RegisterUtils.h"
#include "Phobos/Game/Physics/Manager.h"

namespace
{
	struct ShapeTypeName_s
	{
		const char *pstrzName;
		Phobos::Game::Physics::CollisionShapeTypes_t eValue;
	};

	static const ShapeTypeName_s stShapeTypeNameTable_gl[] = 
	{
		{"box", Phobos::Game::Physics::CollisionShapeTypes::BOX},
		{"sphere", Phobos::Game::Physics::CollisionShapeTypes::SPHERE},
		{"mesh", Phobos::Game::Physics::CollisionShapeTypes::MESH},
		{"capsule", Phobos::Game::Physics::CollisionShapeTypes::CAPSULE},
		{"cylinderX", Phobos::Game::Physics::CollisionShapeTypes::CYLINDER_X},
		{"cylinderY", Phobos::Game::Physics::CollisionShapeTypes::CYLINDER_Y},
		{"cylinderZ", Phobos::Game::Physics::CollisionShapeTypes::CYLINDER_Z},

		{NULL, Phobos::Game::Physics::CollisionShapeTypes::BOX}
	};
}
			
Phobos::Game::Physics::CollisionShapePtr_t Phobos::Game::Physics::Utils::CreateCollisionShape(const Phobos::Register::Table &collisionDef, const Ogre::Vector3 &scale)
{
	Enum<Phobos::Game::Physics::CollisionShapeTypes_t,  ShapeTypeName_s> enumMap(stShapeTypeNameTable_gl);

	Physics::CollisionShapeTypes_t type;
	if(!enumMap.TryGetValue(type, collisionDef.GetString("colliderType")))
	{
		std::stringstream stream;
		stream << "Invalid colliderType " << collisionDef.GetString("colliderType");
		PH_RAISE(INVALID_PARAMETER_EXCEPTION, "FinishRigidBodyLoad", stream.str());
	}

	CollisionShapePtr_t shape;

	auto &physicsManager = Physics::Manager::GetInstance();

	switch(type)
	{
		case Physics::CollisionShapeTypes::BOX:
			{
				Ogre::Vector3 boxDimensions = Register::GetVector3(collisionDef, "boxDimensions") * scale;
				shape = physicsManager.CreateBoxShape(boxDimensions.x, boxDimensions.y, boxDimensions.z );
			}
			break;

		case Physics::CollisionShapeTypes::CYLINDER_X:
		case Physics::CollisionShapeTypes::CYLINDER_Y:
		case Physics::CollisionShapeTypes::CYLINDER_Z:
		case Physics::CollisionShapeTypes::CAPSULE:
			{
				Float_t radius = collisionDef.GetFloat("radius");
				Float_t height = collisionDef.GetFloat("height");

				switch(type)
				{
					case Physics::CollisionShapeTypes::CYLINDER_X:
						shape = physicsManager.CreateCylinderShapeX(radius, height);
						break;

					case Physics::CollisionShapeTypes::CYLINDER_Y:
						shape = physicsManager.CreateCylinderShapeY(radius, height);
						break;

					case Physics::CollisionShapeTypes::CYLINDER_Z:
						shape = physicsManager.CreateCylinderShapeZ(radius, height);
						break;

					case Physics::CollisionShapeTypes::CAPSULE:
						shape = physicsManager.CreateCapsuleShape(radius, height);
						break;
				}
			}
			break;

#ifdef false
		case Manager::CST_MESH:
			{
				//Ogre::MeshPtr mesh = Ogre::ResourceManager::ins
				//spRigidBody = physicsManager->CreateMeshRigidBody(bodyType, transform, mass, collisionTag, boxDimensions.x, boxDimensions.y, boxDimensions.z );
			}
#endif

		default:
			PH_RAISE(INVALID_PARAMETER_EXCEPTION, "RigidBodyComponent::OnLoad", "Shape not supported");
			break;
	}

	return shape;
}

