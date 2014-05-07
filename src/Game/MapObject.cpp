#include "Phobos/Game/MapObject.h"

#include <Phobos/Error.h>
#include <Phobos/OgreEngine/Math/Transform.h>
#include <Phobos/OgreEngine/Render.h>

#include <OgreSceneNode.h>
#include <OgreEntity.h>

namespace Phobos
{
	namespace Game
	{
		MapObject::Data::~Data()
		{
			auto &render = Phobos::OgreEngine::Render::GetInstance();

			if (m_pclSceneNode)
			{
				render.DestroySceneNode(m_pclSceneNode);
				m_pclSceneNode = NULL;
			}

			if (m_pclLight)
			{
				render.DestroyLight(m_pclLight);
				m_pclLight = NULL;
			}

			if (m_pclEntity)
			{
				render.DestroyEntity(m_pclEntity);
				m_pclEntity = NULL;
			}
		}

		void MapObject::Data::AttachLight(Ogre::Light *light)
		{
			PH_ASSERT(!m_pclLight);

			m_pclLight = light;
			m_pclSceneNode->attachObject(m_pclLight);
		}

		void MapObject::Data::AttachEntity(Ogre::Entity *entity)
		{
			PH_ASSERT(!m_pclEntity);

			m_pclEntity = entity;
			m_pclSceneNode->attachObject(entity);
		}

#if 0
		void MapObject::Data::SetParentNode(Data &node)
		{
			PH_ASSERT(node.m_pclSceneNode);
			PH_ASSERT(m_pclSceneNode);

			m_pclSceneNode->getParent()->removeChild(m_pclSceneNode);
			node.m_pclSceneNode->addChild(m_pclSceneNode);
		}
#endif

		//Why using those _ functions: http://89.151.96.106/forums/viewtopic.php?f=22&t=62386
		//http://www.ogre3d.org/forums/viewtopic.php?p=221113
		//http://www.ogre3d.org/tikiwiki/-SceneNode
		const Ogre::Vector3 &MapObject::Data::GetWorldPosition() const
		{
			return m_pclSceneNode->_getDerivedPosition();
		}

		const Ogre::Vector3 &MapObject::Data::GetWorldScale() const
		{
			return m_pclSceneNode->_getDerivedScale();
		}

		const Ogre::Quaternion &MapObject::Data::GetWorldOrientation() const
		{
			return m_pclSceneNode->_getDerivedOrientation();
		}

		void MapObject::Data::SetPosition(const Ogre::Vector3 &position)
		{
			m_pclSceneNode->setPosition(position);
		}

		void MapObject::Data::SetOrientation(const Ogre::Quaternion &orientation)
		{
			m_pclSceneNode->setOrientation(orientation);
		}

		void MapObject::Data::RegisterBody()
		{
			m_clRigidBody.Register();
		}

		void MapObject::SyncSceneToPhysics()
		{
			PH_ASSERT(m_clData.m_ePhysicsType != PhysicsTypes::NONE);
			
			const auto transform = m_clData.m_clRigidBody.GetTransform();

			m_clData.m_pclSceneNode->setOrientation(transform.GetRotation());
			m_clData.m_pclSceneNode->setPosition(transform.GetOrigin());			
		}

		void MapObject::SyncPhysicsToScene()
		{
			PH_ASSERT(m_clData.m_ePhysicsType != PhysicsTypes::NONE);

			Engine::Math::Transform transform(this->GetWorldPosition(), this->GetWorldOrientation());

			m_clData.m_clRigidBody.Warp(transform);
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
	}
}
