#include "Phobos/Game/Level/MeshComponent.h"

#include <Phobos/OgreEngine/Render.h>
#include <Phobos/Register/Table.h>

#include "Phobos/Game/Level/ComponentPool.h"
#include "Phobos/Game/Level/MapDefs.h"
#include "Phobos/Game/Level/MapObject.h"
#include "Phobos/Game/Level/MapObjectComponentAccess.h"
#include "Phobos/Game/Level/MapObjectComponentFactory.h"

#include <boost/pool/object_pool.hpp>

#include <OgreEntity.h>
#include <OgreSkeletonInstance.h>

#define PH_MESH_COMPONENT_NAME "Mesh"

PH_GAME_DEFINE_COMPONENT_POOL(Phobos::Game::MeshComponent, g_poolMeshes);

namespace Phobos
{
	namespace Game
	{
		PH_MAP_COMPONENT_FULL_CREATOR(PH_MESH_COMPONENT_NAME, MeshComponent);

		static const String_t g_strComponentTypeName(PH_MESH_COMPONENT_NAME);
		
		const String_t &MeshComponent::GetComponentName()
		{
			return g_strComponentTypeName;
		}
		

		MeshComponent::MeshComponent(MapObject &owner, const Register::Table &table) :
			MapObjectComponent(PH_MESH_COMPONENT_NAME, owner),
			m_pclEntity(OgreEngine::Render::GetInstance().CreateEntity(table.GetString(PH_MAP_OBJECT_KEY_MESH)))
		{
			this->AccessMapObject().AttachOgreObject(*m_pclEntity);			
		}

		MeshComponent::~MeshComponent()
		{
			OgreEngine::Render::GetInstance().DestroyEntity(m_pclEntity);
		}

		void MeshComponent::AttachObjectToBone(
			const Char_t *boneName,
			Ogre::MovableObject &movable,
			const Ogre::Quaternion &offsetOrientation,
			const Ogre::Vector3 &offsetPosition
		)
		{
			m_pclEntity->attachObjectToBone(boneName, &movable, offsetOrientation, offsetPosition);
		}

		void MeshComponent::AttachObjectToBone(
			const String_t &boneName,
			Ogre::MovableObject &movable,
			const Ogre::Quaternion &offsetOrientation,
			const Ogre::Vector3 &offsetPosition
			)
		{
			m_pclEntity->attachObjectToBone(boneName, &movable, offsetOrientation, offsetPosition);
		}

		Ogre::Bone &MeshComponent::GetBone(const char *boneName)
		{
			return *m_pclEntity->getSkeleton()->getBone(boneName);
		}

		Ogre::Bone &MeshComponent::GetBone(const String_t &boneName)
		{
			return *m_pclEntity->getSkeleton()->getBone(boneName);
		}
	}
}


