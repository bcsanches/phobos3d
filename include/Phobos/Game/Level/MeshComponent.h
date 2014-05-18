#ifndef PH_PHOBOS_GAME_LEVEL_MESH_COMPONENT_H
#define PH_PHOBOS_GAME_LEVEL_MESH_COMPONENT_H

#include "Phobos/Game/Level/MapObjectComponent.h"

#include <Phobos/Register/TableFwd.h>

#include <OgrePrerequisites.h>

namespace Phobos
{
	namespace Game
	{
		class MeshComponent : public MapObjectComponent
		{
			public:
				MeshComponent(MapObject &owner, const Register::Table &table);
				virtual ~MeshComponent();

				static MapObjectComponent::UniquePtr_t Create(MapObject &owner, const Register::Table &table);

			protected:
				virtual void Release() override;

			private:
				Ogre::Entity *m_pclEntity;
		};
	}
}

#endif