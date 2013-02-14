/*
Phobos 3d
February 2011
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

#include "PH_ModelRendererComponent.h"

#include <OgreEntity.h>

#include <Phobos/Register/Table.h>

#include <PH_Render.h>
#include <PH_TransformProperty.h>

#include "PH_BaseOgreGameWorld.h"
#include "PH_GameRegisterUtils.h"
#include "PH_EntityComponentFactory.h"
#include "PH_EntityKeys.h"
#include "PH_ModelRendererManager.h"

namespace Phobos
{
	PH_FULL_ENTITY_COMPONENT_CREATOR(PH_MODEL_RENDERER_COMPONENT_NAME, ModelRendererComponent);

	EntityInputManager ModelRendererComponent::clInputManager_gl;
		
	ModelRendererComponent::ModelRendererComponent(const String_t &name, Entity &owner):
		EntityComponent(name, owner),
		m_pclSceneNode(NULL),
		m_pclMeshEntity(NULL),
		m_pprpTransform(NULL)
	{		
		ModelRendererManager::GetInstance().Register(*this);
	}

	ModelRendererComponent::~ModelRendererComponent()
	{
		Render &render = Render::GetInstance();

		render.DestroyEntity(m_pclMeshEntity);
		render.DestroySceneNode(m_pclSceneNode);

		ModelRendererManager::GetInstance().Unregister(*this);
	}

	void ModelRendererComponent::OnLoad(const Register::Table &table)
	{
		const String_t &meshName = table.GetString(PH_ENTITY_KEY_MESH_FILE);

		Render &render = Render::GetInstance();

		m_pclSceneNode = render.CreateSceneNode(this->GetEntityName());
		m_pclMeshEntity = render.CreateEntity(meshName);

		m_pclSceneNode->attachObject(m_pclMeshEntity);
		
		m_pclSceneNode->setScale(Register::GetVector3(table, PH_ENTITY_KEY_SCALE));		

		m_strParentNode = table.GetString(PH_ENTITY_KEY_PARENT_NODE);
	}

	void ModelRendererComponent::OnLoadFinished()
	{
		EntityComponent::OnLoadFinished();		

		m_pprpTransform = &this->GetCustomEntityProperty<TransformProperty>(PH_ENTITY_PROP_TRANSFORM);

		//Force node transform update
		this->Update();

		if(m_strParentNode != PH_WORLD_SCENE_MANAGER_NAME)
		{
			Render &render = Render::GetInstance();

			m_pclSceneNode->getParent()->removeChild(m_pclSceneNode);

			render.GetSceneNode(m_strParentNode)->addChild(m_pclSceneNode);
		}		
	}

	void ModelRendererComponent::Update()
	{		
		m_pclSceneNode->setPosition(m_pprpTransform->GetOrigin());
		m_pclSceneNode->setOrientation(m_pprpTransform->GetRotation());
	}

	void ModelRendererComponent::AttachObjectToBone(
				const Char_t *boneName, 
				Ogre::MovableObject &movable, 
				const Ogre::Quaternion &offsetOrientation, 
				const Ogre::Vector3 &offsetPosition
			)
	{
		m_pclMeshEntity->attachObjectToBone(boneName, &movable, offsetOrientation, offsetPosition);
	}

	void ModelRendererComponent::AttachObjectToBone(
				const String_t &boneName, 
				Ogre::MovableObject &movable, 
				const Ogre::Quaternion &offsetOrientation, 
				const Ogre::Vector3 &offsetPosition
			)
	{
		m_pclMeshEntity->attachObjectToBone(boneName, &movable, offsetOrientation, offsetPosition);
	}

	Ogre::Bone &ModelRendererComponent::GetBone(const char *boneName)
	{
		return *m_pclMeshEntity->getSkeleton()->getBone(boneName);
	}

	Ogre::Bone &ModelRendererComponent::GetBone(const String_t &boneName)
	{
		return *m_pclMeshEntity->getSkeleton()->getBone(boneName);
	}

	PH_BEGIN_ENTITY_INPUT(ModelRendererComponent, SetPosition)
	{

	}
}
