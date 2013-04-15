/*
Phobos 3d
September 2011
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
#include "PH_OgitorGameWorld.h"

#include <OgreEntity.h>
#include <OgreLight.h>
#include <OgreSceneNode.h>

#include <Phobos/Register/Table.h>
#include <Phobos/Register/Hive.h>
#include <Phobos/Register/Manager.h>

#include <Phobos/Exception.h>
#include <Phobos/Log.h>
#include <Phobos/OgreEngine/Render.h>
#include <Phobos/OgreEngine/Math/Transform.h>

#include "PH_CollisionTag.h"
#include "PH_GameRegisterUtils.h"
#include "PH_EntityFactory.h"
#include "PH_EntityKeys.h"
#include "PH_GamePhysicsSettings.h"
#include "PH_MapLoader.h"
#include "PH_PhysicsManager.h"
#include "PH_RigidBody.h"

#include "PH_GameEventManager.h"

namespace Phobos
{
	OgitorGameWorld::OgitorGameWorld():		
		m_pclTerrainGroup(NULL),
		m_pclTerrainOptions(NULL),
		m_pclTerrainLight(NULL),
		m_pclTerrainGroupTable(NULL),
		m_pclTerrainPageTable(NULL)
	{
		//empty
	}

	OgitorGameWorld::~OgitorGameWorld()
	{		
		if(m_pclTerrainGroup)
			OgreEngine::Render::GetInstance().DestroyTerrainGroup(m_pclTerrainGroup);

		if(m_pclTerrainOptions)
			delete m_pclTerrainOptions;		

		for(auto &pair : m_mapStaticObjects)
		{
			pair.second.Clear();			
		}
	}

	void OgitorGameWorld::Load(const MapLoader &loader, const Register::Table &worldEntityDef)
	{
		const auto &hive = loader.GetStaticEntitiesHive();

		for(auto it : hive)		
		{
			Register::Table *dict = static_cast<Register::Table *>(it.second);

			try
			{
				const String_t &name = dict->GetString("name");
				const String_t &type = dict->GetString("typename");

				if(this->LoadGlobalObject(type, *dict))
					continue;

				//make sure this is not duplicated
				StaticObjectsMap_t::iterator objIt = m_mapStaticObjects.lower_bound(name);
				if((objIt != m_mapStaticObjects.end()) && !(m_mapStaticObjects.key_comp()(name, objIt->first)))
				{
					LogMakeStream() << "[OgitorGameWorld::Load] Static object " << name << " is duplicated\n";
					continue;
				}

				StaticObject_s object;
				if(!this->LoadStaticObject(object, name, type, *dict))
					continue;

				m_mapStaticObjects.insert(objIt, std::make_pair(name, object));

			}
			catch(Exception &ex)
			{
				LogMakeStream() << "[OgitorGameWorld::Load] Exception loading static object: " << ex.what();
			}
		}

		//time to fix parents
		for(StaticObjectsMap_t::value_type &pair : m_mapStaticObjects)
		{
			StaticObject_s &object = pair.second;

			if(StringIsBlank(object.strParent) || (object.strParent.compare("SceneManager") == 0))
				continue;

			StaticObjectsMap_t::iterator it = m_mapStaticObjects.find(object.strParent);
			if(it == m_mapStaticObjects.end())
			{
				LogMakeStream() << "[OgitorGameWorld::Load] Static object " << pair.first << " without parent " << object.strParent << "\n";
				continue;
			}

			if(object.m_pclSceneNode->getParent())
				object.m_pclSceneNode->getParent()->removeChild(object.m_pclSceneNode);
			it->second.m_pclSceneNode->addChild(object.m_pclSceneNode);
		}

		//configure physics
		Physics::Manager &physicsManager = Physics::Manager::GetInstance();

		Physics::CollisionTag staticCollisionTag = GamePhysicsSettings::CreateStaticWorldCollisionTag();

		for(auto &pair : m_mapStaticObjects)
		{
			StaticObject_s &object = pair.second;

			if(object.m_pclEntity == NULL)
				continue;

			//Why using those _ functions: http://89.151.96.106/forums/viewtopic.php?f=22&t=62386
			//http://www.ogre3d.org/forums/viewtopic.php?p=221113
			//http://www.ogre3d.org/tikiwiki/-SceneNode
			Engine::Math::Transform transform(
				object.m_pclSceneNode->_getDerivedPosition(), 
				object.m_pclSceneNode->_getDerivedOrientation()
			);

			BaseOgreGameWorld::CreateStaticObjectRigidBody(object, transform, object.m_pclSceneNode->_getDerivedScale(), staticCollisionTag);
		}
		

		if(m_pclTerrainGroupTable != NULL)
		{
			this->LoadTerrainGroup(*m_pclTerrainGroupTable);
			m_pclTerrainGroupTable = NULL;
		}

		if(m_pclTerrainPageTable != NULL)
		{
			this->LoadTerrainPage(*m_pclTerrainPageTable, worldEntityDef);
			m_pclTerrainPageTable = NULL;
		}		
	}

	bool OgitorGameWorld::LoadGlobalObject(const String_t &type, const Register::Table &dict)
	{
		if((type.compare("Caelum Object") == 0) ||
		   (type.compare("Viewport Object") == 0))
		{
			return true;
		}
		else if(type.compare("OctreeSceneManager") == 0)
		{
			auto &render = OgreEngine::Render::GetInstance();
			render.SetAmbientColor(Register::GetColour(dict, "ambient"));

			return true;
		}
		else if(type.compare("Terrain Group Object") == 0)
		{
			m_pclTerrainGroupTable = &dict;

			return true;
		}
		else if(type.compare("Terrain Page Object") == 0)
		{
			m_pclTerrainPageTable = &dict;

			return true;
		}

		return false;
	}

	void OgitorGameWorld::LoadTerrainGroup(const Register::Table &dict)
	{
		m_pclTerrainOptions = new Ogre::TerrainGlobalOptions();

		m_pclTerrainOptions->setMaxPixelError(dict.GetFloat("tuning::maxpixelerror"));
		m_pclTerrainOptions->setLightMapSize(dict.GetInt("lightmap::texturesize"));
		m_pclTerrainOptions->setCompositeMapSize(dict.GetInt("tuning::compositemaptexturesize"));
		m_pclTerrainOptions->setCompositeMapDistance(dict.GetFloat("tuning::compositemapdistance"));
		m_pclTerrainOptions->setLayerBlendMapSize(dict.GetInt("blendmap::texturesize"));

		auto &render = OgreEngine::Render::GetInstance();

		//u16TerrainSize = dict.GetInt("pagemapsize");
		//pclTerrainGroup = render->CreateTerrainGroup(Ogre::Terrain::ALIGN_X_Z, u16TerrainSize, dict.GetFloat("pageworldsize"));
		m_pclTerrainGroup = render.CreateTerrainGroup(Ogre::Terrain::ALIGN_X_Z, dict.GetInt("pagemapsize"), dict.GetFloat("pageworldsize"));
		m_pclTerrainGroup->setOrigin(Ogre::Vector3::ZERO);
		m_pclTerrainGroup->setFilenameConvention("Page", "ogt");

		if(m_pclTerrainLight != NULL)
		{
			m_pclTerrainOptions->setLightMapDirection(m_pclTerrainLight->getDirection());
			m_pclTerrainOptions->setCompositeMapAmbient(render.GetAmbientColor());
			m_pclTerrainOptions->setCompositeMapDiffuse(m_pclTerrainLight->getDiffuseColour());
		}
	}

	void OgitorGameWorld::LoadTerrainPage(const Register::Table &terrainPageTable, const Register::Table &worldEntityDef)
	{
		auto &levelInfo = Register::GetHive("LevelInfo");

		String_t name = levelInfo.GetTable("LevelFile").GetString("path") + "/" + worldEntityDef.GetString("terrainDir") + "/" + m_pclTerrainGroup->generateFilename(0, 0);
		m_pclTerrainGroup->defineTerrain(0, 0, name);
		m_pclTerrainGroup->loadTerrain(0, 0, true);
	}

	bool OgitorGameWorld::LoadStaticObject(StaticObject_s &object, const String_t &name, const String_t &type, const Register::Table &dict)
	{
		TempStaticObject_s temp;

		temp.m_fParent = dict.TryGetString(PH_ENTITY_KEY_PARENT_NODE, object.strParent) && (object.strParent.compare(PH_WORLD_SCENE_MANAGER_NAME) != 0);
		temp.m_strName = name;

		if(type.compare("Node Object") == 0)
		{
			this->LoadNodeObject(temp, dict);
		}
		else if(type.compare("Light Object") == 0)
		{
			this->LoadLightObject(temp, dict);
		}
		else if(type.compare("Entity Object") == 0)
		{
			this->LoadEntityObject(temp, dict);
		}
		else
		{
			LogMakeStream() << "[OgitorGameWorld::LoadStaticObject] Error, unknown static object type: " << type << "\n";
			return false;
		}

		temp.Commit(object);

		//Check if we have a directional light after commit, to avoid dangling pointers
		if((object.m_pclLight != NULL) && (object.m_pclLight->getType() == Ogre::Light::LT_DIRECTIONAL))
			m_pclTerrainLight = object.m_pclLight;

		return true;
	}

	void OgitorGameWorld::LoadNodeObject(TempStaticObject_s &temp, const Register::Table &dict)
	{
		temp.m_pclSceneNode = OgreEngine::Render::GetInstance().CreateSceneNode(temp.m_strName);

		temp.m_pclSceneNode->setPosition(Register::GetVector3(dict, PH_ENTITY_KEY_POSITION));
		temp.m_pclSceneNode->setOrientation(Register::GetQuaternion(dict, PH_ENTITY_KEY_ORIENTATION));
	}

	void OgitorGameWorld::LoadEntityObject(TempStaticObject_s &temp, const Register::Table &dict)
	{
		this->LoadNodeObject(temp, dict);

		temp.m_pclEntity = OgreEngine::Render::GetInstance().CreateEntity(dict.GetString("meshfile"));
		temp.m_pclEntity->setCastShadows(dict.GetBool("castshadows"));
		temp.m_pclSceneNode->attachObject(temp.m_pclEntity);		
	}

	void OgitorGameWorld::LoadLightObject(TempStaticObject_s &temp, const Register::Table &dict)
	{
		temp.m_pclLight = OgreEngine::Render::GetInstance().CreateLight();

		if(temp.m_fParent)
		{
			this->LoadNodeObject(temp, dict);
			temp.m_pclSceneNode->attachObject(temp.m_pclLight);
		}

		temp.m_pclLight->setCastShadows(dict.GetBool("castshadows"));

		switch(dict.GetInt("lighttype"))
		{
			case 0:
				temp.m_pclLight->setType(Ogre::Light::LT_POINT);
				if(!temp.m_fParent)
					temp.m_pclLight->setPosition(Register::GetVector3(dict, PH_ENTITY_KEY_POSITION));
				break;

			case 1:
				temp.m_pclLight->setType(Ogre::Light::LT_DIRECTIONAL);
				break;

			case 2:
				{
					temp.m_pclLight->setType(Ogre::Light::LT_SPOTLIGHT);

					if(!temp.m_fParent)
						temp.m_pclLight->setPosition(Register::GetVector3(dict, PH_ENTITY_KEY_POSITION));

					Ogre::Vector3 lightRange = Register::GetVector3(dict, "lightrange");
					temp.m_pclLight->setSpotlightRange(Ogre::Degree(lightRange.x), Ogre::Degree(lightRange.y), lightRange.z);
				}
				break;

			default:
				{
					std::stringstream stream;

					stream << "Invalid light type " << dict.GetInt("lighttype") << " for object " << dict.GetString("name");
					PH_RAISE(INVALID_PARAMETER_EXCEPTION, "OgitorGameWorld::LoadLightObject", stream.str());
				}
				break;
		}

		float attenuation[4];
		dict.Get4Float(attenuation, "attenuation");
		temp.m_pclLight->setAttenuation(attenuation[0], attenuation[1], attenuation[2], attenuation[3]);

		temp.m_pclLight->setDiffuseColour(Register::GetColour(dict, "diffuse"));

		if(!temp.m_fParent)
			temp.m_pclLight->setDirection(Register::GetVector3(dict, "direction"));

		temp.m_pclLight->setPowerScale(dict.GetFloat("power"));
		temp.m_pclLight->setSpecularColour(Register::GetColour(dict, "specular"));		
	}		
}
