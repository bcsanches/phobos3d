#ifndef PH_PHOBOS_GAME_LEVEL_LIGHT_COMPONENT_H
#define PH_PHOBOS_GAME_LEVEL_LIGHT_COMPONENT_H

#include "Phobos/Game/Level/MapObjectComponent.h"

#include <Phobos/Register/TableFwd.h>

#include <OgrePrerequisites.h>

#define PH_LIGHT_COMPONENT_NAME "Light"

namespace Phobos
{
	namespace Game
	{
		class LightComponent : public MapObjectComponent
		{
			public:
				LightComponent(MapObject &owner, const Register::Table &table);
				virtual ~LightComponent();

				static MapObjectComponent::UniquePtr_t Create(MapObject &owner, const Register::Table &table);

				bool IsDirectional();

				Ogre::Vector3 GetDirection();
				Ogre::ColourValue GetDiffuseColour();

			protected:
				virtual void Release() override;

			private:
				Ogre::Light *m_pclLight;
		};
	}
}

#endif
