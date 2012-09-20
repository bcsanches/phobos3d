/*
Phobos 3d
May 2012
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

#include "PH_BaseOgreGameWorld.h"

#include <OgreEntity.h>

#include <PH_Path.h>
#include <PH_Render.h>

#include "PH_GamePhysicsSettings.h"
#include "PH_PhysicsManager.h"
#include "PH_PhysicsUtils.h"
#include "PH_RigidBody.h"

namespace Phobos
{
	void BaseOgreGameWorld_c::StaticObject_s::Clear()
	{
		Render_c &render = Render_c::GetInstance();

		if(pclSceneNode)
		{
			render.DestroySceneNode(pclSceneNode);
			pclSceneNode = NULL;
		}

		if(pclLight)
		{
			render.DestroyLight(pclLight);
			pclLight = NULL;
		}

		if(pclEntity)
		{
			render.DestroyEntity(pclEntity);
			pclEntity = NULL;
		}

		spRigidBody.reset();		
	}

	BaseOgreGameWorld_c::TempStaticObject_s::~TempStaticObject_s()
	{
		Render_c &render = Render_c::GetInstance();

		if(pclSceneNode)
			render.DestroySceneNode(pclSceneNode);

		if(pclLight)
			render.DestroyLight(pclLight);

		if(pclEntity)
			render.DestroyEntity(pclEntity);
	}

	void BaseOgreGameWorld_c::CreateStaticObjectRigidBody(StaticObject_s &staticObj, const Transform_c &transform, const Ogre::Vector3 &scale, const Physics::CollisionTag_c &collisionTag) const
	{		
		const Ogre::MeshPtr mesh = staticObj.pclEntity->getMesh();
		const String_c &meshName = mesh->getName();
		
		Path_c path(meshName);
		path.StripExtension();

		Physics::Manager_c &physicsManager = Physics::Manager_c::GetInstance();

		const Dictionary_c *collisionDef = GamePhysicsSettings_c::TryGetStaticMeshCollisionShapeDef(path.GetStr());
		if(collisionDef != NULL)
		{
			staticObj.spRigidBody = physicsManager.CreateRigidBody(Physics::RBT_STATIC, transform, 0, collisionTag, Physics::Utils::CreateCollisionShape(*collisionDef, scale));
		}
		else
		{			
			staticObj.spRigidBody = physicsManager.CreateMeshRigidBody(Physics::RBT_STATIC, transform, 0, collisionTag, *staticObj.pclEntity->getMesh(), scale);					
		}

		staticObj.spRigidBody->Register();
	}
}
