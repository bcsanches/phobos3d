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

#include "Phobos/Game/BaseOgreWorld.h"

#include <OgreEntity.h>

#include <Phobos/Path.h>
#include <Phobos/OgreEngine/Render.h>

#include "Phobos/Game/Physics/Settings.h"
#include "Phobos/Game/Physics/Manager.h"
#include "Phobos/Game/Physics/RigidBody.h"
#include "Phobos/Game/Physics/PhysicsUtils.h"

void Phobos::Game::BaseOgreWorld::StaticObject_s::Clear()
{
	auto &render = OgreEngine::Render::GetInstance();

	if(m_pclSceneNode)
	{
		render.DestroySceneNode(m_pclSceneNode);
		m_pclSceneNode = NULL;
	}

	if(m_pclLight)
	{
		render.DestroyLight(m_pclLight);
		m_pclLight = NULL;
	}

	if(m_pclEntity)
	{
		render.DestroyEntity(m_pclEntity);
		m_pclEntity = NULL;
	}

	m_spRigidBody.reset();		
}

Phobos::Game::BaseOgreWorld::TempStaticObject_s::~TempStaticObject_s()
{
	auto &render = OgreEngine::Render::GetInstance();

	if(m_pclSceneNode)
		render.DestroySceneNode(m_pclSceneNode);

	if(m_pclLight)
		render.DestroyLight(m_pclLight);

	if(m_pclEntity)
		render.DestroyEntity(m_pclEntity);
}

void Phobos::Game::BaseOgreWorld::CreateStaticObjectRigidBody(StaticObject_s &staticObj, const Engine::Math::Transform &transform, const Ogre::Vector3 &scale, const Physics::CollisionTag &collisionTag) const
{		
	const Ogre::MeshPtr mesh = staticObj.m_pclEntity->getMesh();
	const String_t &meshName = mesh->getName();
		
	Path path(meshName);
	path.StripExtension();

	Physics::Manager &physicsManager = Physics::Manager::GetInstance();

	auto collisionDef = Physics::Settings::TryGetStaticMeshCollisionShapeDef(path.GetStr());
	if(collisionDef != NULL)
	{
		staticObj.m_spRigidBody = physicsManager.CreateRigidBody(Physics::RBT_STATIC, transform, 0, collisionTag, Physics::Utils::CreateCollisionShape(*collisionDef, scale));
	}
	else
	{			
		staticObj.m_spRigidBody = physicsManager.CreateMeshRigidBody(Physics::RBT_STATIC, transform, 0, collisionTag, *staticObj.m_pclEntity->getMesh(), scale);					
	}

	staticObj.m_spRigidBody->Register();
}

