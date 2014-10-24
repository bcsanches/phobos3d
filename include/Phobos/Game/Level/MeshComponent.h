#ifndef PH_PHOBOS_GAME_LEVEL_MESH_COMPONENT_H
#define PH_PHOBOS_GAME_LEVEL_MESH_COMPONENT_H

#include "Phobos/Game/Level/MapObjectComponent.h"

#include "Phobos/Game/GameAPI.h"

#include <Phobos/Register/TableFwd.h>

#include <OgrePrerequisites.h>
#include <OgreVector3.h>

namespace Phobos
{
	namespace Game
	{
		class PH_GAME_API MeshComponent : public MapObjectComponent
		{
			public:
				MeshComponent(MapObject &owner, const Register::Table &table);
				virtual ~MeshComponent();

				void AttachObjectToBone(
					const Char_t *boneName,
					Ogre::MovableObject &movable,
					const Ogre::Quaternion &offsetOrientation = Ogre::Quaternion::IDENTITY,
					const Ogre::Vector3 &offsetPosition = Ogre::Vector3::ZERO
				);

				void AttachObjectToBone(
					const String_t &boneName,
					Ogre::MovableObject &movable,
					const Ogre::Quaternion &offsetOrientation = Ogre::Quaternion::IDENTITY,
					const Ogre::Vector3 &offsetPosition = Ogre::Vector3::ZERO
				);

				Ogre::Bone &GetBone(const char *boneName);
				Ogre::Bone &GetBone(const String_t &boneName);

				static MapObjectComponent::UniquePtr_t Create(MapObject &owner, const Register::Table &table);

				static const String_t &GetComponentName();

			protected:
				virtual void Release() override;

			private:
				Ogre::Entity *m_pclEntity;
		};
	}
}

#endif