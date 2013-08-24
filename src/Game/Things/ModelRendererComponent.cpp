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

#include "Phobos/Game/Things/ModelRendererComponent.h"

#include <OgreEntity.h>

#include <Phobos/Register/Table.h>

#include <Phobos/OgreEngine/Render.h>
#include <Phobos/OgreEngine/TransformProperty.h>

#include "Phobos/Game/BaseOgreWorld.h"
#include "Phobos/Game/MapDefs.h"
#include "Phobos/Game/RegisterUtils.h"
#include "Phobos/Game/Things/ComponentFactory.h"
#include "Phobos/Game/Things/Keys.h"
#include "Phobos/Game/Things/ModelRendererManager.h"

namespace Phobos
{
	namespace Game
	{
		namespace Things
		{
			PH_FULL_ENTITY_COMPONENT_CREATOR(PH_MODEL_RENDERER_COMPONENT_NAME, ModelRendererComponent);

			//EntityInputManager ModelRendererComponent::clInputManager_gl;
		
			ModelRendererComponent::ModelRendererComponent(const String_t &name, Entity &owner):
				Component(name, owner),
				m_pprpTransform(NULL)
			{		
				ModelRendererManager::GetInstance().Register(*this);
			}

			ModelRendererComponent::~ModelRendererComponent()
			{				
				ModelRendererManager::GetInstance().Unregister(*this);
			}

			void ModelRendererComponent::OnLoad(const Register::Table &table)
			{
				const String_t &renderObjectHandler = table.GetString(PH_GAME_OBJECT_KEY_RENDER_OBJECT_HANDLER);

				m_hSceneNode = RenderWorld::GetInstance().AcquireDynamicSceneNodeHandler(renderObjectHandler);				
			}

			void ModelRendererComponent::OnLoadFinished()
			{
				Component::OnLoadFinished();		

				m_pprpTransform = &this->GetCustomEntityProperty<OgreEngine::TransformProperty>(PH_ENTITY_PROP_TRANSFORM);					
			}

			void ModelRendererComponent::Update()
			{		
				m_hSceneNode.SetPosition(m_pprpTransform->GetOrigin());
				m_hSceneNode.SetOrientation(m_pprpTransform->GetRotation());
			}

#if 0
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
#endif

			/*
			PH_BEGIN_ENTITY_INPUT(ModelRendererComponent, SetPosition)
			{

			}
			*/
		}
	}

}
