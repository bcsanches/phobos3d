#include "Phobos/Game/Level/LightComponent.h"

#include <Phobos/OgreEngine/Render.h>
#include <Phobos/Register/Table.h>

#include "Phobos/Game/Level/ComponentPool.h"
#include "Phobos/Game/Level/MapDefs.h"
#include "Phobos/Game/Level/MapObject.h"
#include "Phobos/Game/Level/MapObjectComponentAccess.h"
#include "Phobos/Game/Level/MapObjectComponentFactory.h"
#include "Phobos/Game/RegisterUtils.h"

#include <boost/pool/object_pool.hpp>

#include <OgreEntity.h>

PH_GAME_DEFINE_COMPONENT_POOL(Phobos::Game::LightComponent, g_poolLights);

namespace Phobos
{
	namespace Game
	{
		PH_MAP_COMPONENT_FULL_CREATOR(PH_LIGHT_COMPONENT_NAME, LightComponent);

		LightComponent::LightComponent(MapObject &owner, const Register::Table &table):
			MapObjectComponent(owner),
			m_pclLight(Phobos::OgreEngine::Render::GetInstance().CreateLight())
		{
			m_pclLight->setCastShadows(table.GetBool(PH_MAP_OBJECT_KEY_CAST_SHADOWS));

			StringRef_t lightType = table.GetString(PH_MAP_OBJECT_KEY_LIGHT_TYPE);

			if (lightType.compare(PH_MAP_OBJECT_LIGHT_TYPE_POINT) == 0)
			{
				m_pclLight->setType(Ogre::Light::LT_POINT);
			}
			else if (lightType.compare(PH_MAP_OBJECT_LIGHT_TYPE_DIRECTIONAL) == 0)
			{
				m_pclLight->setType(Ogre::Light::LT_DIRECTIONAL);
			}
			else if (lightType.compare(PH_MAP_OBJECT_LIGHT_TYPE_SPOT) == 0)
			{
				m_pclLight->setType(Ogre::Light::LT_SPOTLIGHT);
				Ogre::Vector3 lightRange = Register::GetVector3(table, PH_MAP_OBJECT_KEY_LIGHT_RANGE);
				m_pclLight->setSpotlightRange(Ogre::Degree(lightRange.x), Ogre::Degree(lightRange.y), lightRange.z);
			}
			else
			{
				std::stringstream stream;

				stream << "Invalid light type " << lightType;
				PH_RAISE(INVALID_PARAMETER_EXCEPTION, "MapWorld::LoadLight", stream.str());
			}

			float attenuation[4];
			table.Get4Float(attenuation, "attenuation");
			m_pclLight->setAttenuation(attenuation[0], attenuation[1], attenuation[2], attenuation[3]);

			m_pclLight->setDiffuseColour(Register::GetColour(table, "diffuse"));

			if (m_pclLight->getType() != Ogre::Light::LT_POINT)
				m_pclLight->setDirection(Register::GetVector3(table, "direction"));

			m_pclLight->setPowerScale(table.GetFloat("power"));
			m_pclLight->setSpecularColour(Register::GetColour(table, "specular"));

			this->AccessMapObject().AttachOgreObject(*m_pclLight);
		}

		bool LightComponent::IsDirectional()
		{
			return m_pclLight->getType() == Ogre::Light::LT_DIRECTIONAL;
		}

		Ogre::Vector3 LightComponent::GetDirection()
		{
			return m_pclLight->getDirection();
		}

		Ogre::ColourValue LightComponent::GetDiffuseColour()
		{
			return m_pclLight->getDiffuseColour();
		}

		LightComponent::~LightComponent()
		{
			OgreEngine::Render::GetInstance().DestroyLight(m_pclLight);
		}
	}
}


