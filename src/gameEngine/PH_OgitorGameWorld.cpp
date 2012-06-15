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

#include <boost/foreach.hpp>

#include <OgreEntity.h>
#include <OgreLight.h>
#include <OgreSceneNode.h>

#include <PH_Dictionary.h>
#include <PH_DictionaryHive.h>
#include <PH_DictionaryManager.h>
#include <PH_Exception.h>
#include <PH_Kernel.h>
#include <PH_Render.h>
#include <PH_Transform.h>

#include "PH_GameDictionaryUtils.h"
#include "PH_EntityFactory.h"
#include "PH_EntityKeys.h"
#include "PH_MapLoader.h"

#include "PH_GameEventManager.h"

namespace Phobos
{
	OgitorGameWorld_c::OgitorGameWorld_c():		
		pclTerrainGroup(NULL),
		pclTerrainOptions(NULL),
		pclTerrainLight(NULL),
		pclTerrainGroupDictionary(NULL),
		pclTerrainPageDictionary(NULL)
	{
		//empty
	}

	OgitorGameWorld_c::~OgitorGameWorld_c()
	{		
		if(pclTerrainGroup)
			Render_c::GetInstance()->DestroyTerrainGroup(pclTerrainGroup);

		if(pclTerrainOptions)
			delete pclTerrainOptions;		

		BOOST_FOREACH(StaticObjectsMap_t::value_type &pair, mapStaticObjects)
		{
			pair.second.Clear();			
		}
	}

	void OgitorGameWorld_c::Load(const MapLoader_c &loader, const Dictionary_c &worldEntityDictionary)
	{
		const DictionaryHive_c &hive = loader.GetStaticEntitiesHive();

		for(Node_c::const_iterator it = hive.begin(), end = hive.end(); it != end; ++it)
		{
			DictionaryPtr_t dict = boost::static_pointer_cast<Dictionary_c>(it->second);

			try
			{
				const String_c &name = dict->GetString("name");
				const String_c &type = dict->GetString("typename");

				if(this->LoadGlobalObject(type, *dict))
					continue;

				//make sure this is not duplicated
				StaticObjectsMap_t::iterator objIt = mapStaticObjects.lower_bound(name);
				if((objIt != mapStaticObjects.end()) && !(mapStaticObjects.key_comp()(name, objIt->first)))
				{
					Kernel_c::GetInstance().LogStream() << "[OgitorGameWorld_c::Load] Static object " << name << " is duplicated\n";
					continue;
				}

				StaticObject_s object;
				if(!this->LoadStaticObject(object, name, type, *dict))
					continue;

				mapStaticObjects.insert(objIt, std::make_pair(name, object));

			}
			catch(Exception_c &ex)
			{
				Kernel_c::GetInstance().LogStream() << "[OgitorGameWorld_c::Load] Exception loading static object: " << ex.what();
			}
		}

		//time to fix parents
		BOOST_FOREACH(StaticObjectsMap_t::value_type &pair, mapStaticObjects)
		{
			StaticObject_s &object = pair.second;

			if(StringIsBlank(object.strParent) || (object.strParent.compare("SceneManager") == 0))
				continue;

			StaticObjectsMap_t::iterator it = mapStaticObjects.find(object.strParent);
			if(it == mapStaticObjects.end())
			{
				Kernel_c::GetInstance().LogStream() << "[OgitorGameWorld_c::Load] Static object " << pair.first << " without parent " << object.strParent << "\n";
				continue;
			}

			if(object.pclSceneNode->getParent())
				object.pclSceneNode->getParent()->removeChild(object.pclSceneNode);
			it->second.pclSceneNode->addChild(object.pclSceneNode);
		}

		//configure physics
		Physics::PhysicsManagerPtr_t physicsManager = Physics::PhysicsManager_c::GetInstance();

		BOOST_FOREACH(StaticObjectsMap_t::value_type &pair, mapStaticObjects)
		{
			StaticObject_s &object = pair.second;

			if(object.pclEntity == NULL)
				continue;

			//Why using those _ functions: http://89.151.96.106/forums/viewtopic.php?f=22&t=62386
			//http://www.ogre3d.org/forums/viewtopic.php?p=221113
			//http://www.ogre3d.org/tikiwiki/-SceneNode
			Transform_c transform(
				object.pclSceneNode->_getDerivedPosition(), 
				object.pclSceneNode->_getDerivedOrientation()
			);

			object.spCollisionShape = physicsManager->CreateMeshShape(*object.pclEntity->getMesh().get());
			object.pclRigidBody = physicsManager->CreateRigidBody(transform, *object.spCollisionShape.get(), 0);

			physicsManager->RegisterRigidBody(*object.pclRigidBody);
		}		
		

		if(pclTerrainGroupDictionary != NULL)
		{
			this->LoadTerrainGroup(*pclTerrainGroupDictionary);
			pclTerrainGroupDictionary = NULL;
		}

		if(pclTerrainPageDictionary != NULL)
		{
			this->LoadTerrainPage(*pclTerrainPageDictionary, worldEntityDictionary);
			pclTerrainPageDictionary = NULL;
		}		
	}

	bool OgitorGameWorld_c::LoadGlobalObject(const String_c &type, const Dictionary_c &dict)
	{
		if((type.compare("Caelum Object") == 0) ||
		   (type.compare("Viewport Object") == 0))
		{
			return true;
		}
		else if(type.compare("OctreeSceneManager") == 0)
		{
			RenderPtr_t render = Render_c::GetInstance();
			render->SetAmbientColor(DictionaryGetColour(dict, "ambient"));

			return true;
		}
		else if(type.compare("Terrain Group Object") == 0)
		{
			pclTerrainGroupDictionary = &dict;

			return true;
		}
		else if(type.compare("Terrain Page Object") == 0)
		{
			pclTerrainPageDictionary = &dict;

			return true;
		}

		return false;
	}

	void OgitorGameWorld_c::LoadTerrainGroup(const Dictionary_c &dict)
	{
		pclTerrainOptions = new Ogre::TerrainGlobalOptions();

		pclTerrainOptions->setMaxPixelError(dict.GetFloat("tuning::maxpixelerror"));
		pclTerrainOptions->setLightMapSize(dict.GetInt("lightmap::texturesize"));
		pclTerrainOptions->setCompositeMapSize(dict.GetInt("tuning::compositemaptexturesize"));
		pclTerrainOptions->setCompositeMapDistance(dict.GetFloat("tuning::compositemapdistance"));
		pclTerrainOptions->setLayerBlendMapSize(dict.GetInt("blendmap::texturesize"));

		RenderPtr_t render = Render_c::GetInstance();

		//u16TerrainSize = dict.GetInt("pagemapsize");
		//pclTerrainGroup = render->CreateTerrainGroup(Ogre::Terrain::ALIGN_X_Z, u16TerrainSize, dict.GetFloat("pageworldsize"));
		pclTerrainGroup = render->CreateTerrainGroup(Ogre::Terrain::ALIGN_X_Z, dict.GetInt("pagemapsize"), dict.GetFloat("pageworldsize"));
		pclTerrainGroup->setOrigin(Ogre::Vector3::ZERO);
		pclTerrainGroup->setFilenameConvention("Page", "ogt");

		if(pclTerrainLight != NULL)
		{
			pclTerrainOptions->setLightMapDirection(pclTerrainLight->getDirection());
			pclTerrainOptions->setCompositeMapAmbient(render->GetAmbientColor());
			pclTerrainOptions->setCompositeMapDiffuse(pclTerrainLight->getDiffuseColour());
		}
	}

	void OgitorGameWorld_c::LoadTerrainPage(const Dictionary_c &terrainPageDictionary, const Dictionary_c &worldEntityDictionary)
	{
		DictionaryHivePtr_t levelInfo = DictionaryManager_c::GetInstance()-> GetDictionaryHive("LevelInfo");

		String_c name = levelInfo->GetDictionary("LevelFile")->GetString("path") + "/" + worldEntityDictionary.GetString("terrainDir") + "/" + pclTerrainGroup->generateFilename(0, 0);
		pclTerrainGroup->defineTerrain(0, 0, name);
		pclTerrainGroup->loadTerrain(0, 0, true);
	}

	bool OgitorGameWorld_c::LoadStaticObject(StaticObject_s &object, const String_c &name, const String_c &type, const Dictionary_c &dict)
	{
		TempStaticObject_s temp;

		temp.fParent = dict.TryGetString(PH_ENTITY_KEY_PARENT_NODE, object.strParent) && (object.strParent.compare(PH_WORLD_SCENE_MANAGER_NAME) != 0);
		temp.strName = name;

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
			Kernel_c::GetInstance().LogStream() << "[OgitorGameWorld_c::LoadStaticObject] Error, unknown static object type: " << type << "\n";
			return false;
		}

		temp.Commit(object);

		//Check if we have a directional light after commit, to avoid dangling pointers
		if((object.pclLight != NULL) && (object.pclLight->getType() == Ogre::Light::LT_DIRECTIONAL))
			pclTerrainLight = object.pclLight;

		return true;
	}

	void OgitorGameWorld_c::LoadNodeObject(TempStaticObject_s &temp, const Dictionary_c &dict)
	{
		temp.pclSceneNode = Render_c::GetInstance()->CreateSceneNode(temp.strName);

		temp.pclSceneNode->setPosition(DictionaryGetVector3(dict, PH_ENTITY_KEY_POSITION));
		temp.pclSceneNode->setOrientation(DictionaryGetQuaternion(dict, PH_ENTITY_KEY_ORIENTATION));
	}

	void OgitorGameWorld_c::LoadEntityObject(TempStaticObject_s &temp, const Dictionary_c &dict)
	{
		this->LoadNodeObject(temp, dict);

		temp.pclEntity = Render_c::GetInstance()->CreateEntity(dict.GetString("meshfile"));
		temp.pclEntity->setCastShadows(dict.GetBool("castshadows"));
		temp.pclSceneNode->attachObject(temp.pclEntity);		
	}

	void OgitorGameWorld_c::LoadLightObject(TempStaticObject_s &temp, const Dictionary_c &dict)
	{
		temp.pclLight = Render_c::GetInstance()->CreateLight();

		if(temp.fParent)
		{
			this->LoadNodeObject(temp, dict);
			temp.pclSceneNode->attachObject(temp.pclLight);
		}

		temp.pclLight->setCastShadows(dict.GetBool("castshadows"));

		switch(dict.GetInt("lighttype"))
		{
			case 0:
				temp.pclLight->setType(Ogre::Light::LT_POINT);
				if(!temp.fParent)
					temp.pclLight->setPosition(DictionaryGetVector3(dict, PH_ENTITY_KEY_POSITION));
				break;

			case 1:
				temp.pclLight->setType(Ogre::Light::LT_DIRECTIONAL);
				break;

			case 2:
				{
					temp.pclLight->setType(Ogre::Light::LT_SPOTLIGHT);

					if(!temp.fParent)
						temp.pclLight->setPosition(DictionaryGetVector3(dict, PH_ENTITY_KEY_POSITION));

					Ogre::Vector3 lightRange = DictionaryGetVector3(dict, "lightrange");
					temp.pclLight->setSpotlightRange(Ogre::Degree(lightRange.x), Ogre::Degree(lightRange.y), lightRange.z);
				}
				break;

			default:
				{
					std::stringstream stream;

					stream << "Invalid light type " << dict.GetInt("lighttype") << " for object " << dict.GetString("name");
					PH_RAISE(INVALID_PARAMETER_EXCEPTION, "OgitorGameWorld_c::LoadLightObject", stream.str());
				}
				break;
		}

		float attenuation[4];
		dict.Get4Float(attenuation, "attenuation");
		temp.pclLight->setAttenuation(attenuation[0], attenuation[1], attenuation[2], attenuation[3]);

		temp.pclLight->setDiffuseColour(DictionaryGetColour(dict, "diffuse"));

		if(!temp.fParent)
			temp.pclLight->setDirection(DictionaryGetVector3(dict, "direction"));

		temp.pclLight->setPowerScale(dict.GetFloat("power"));
		temp.pclLight->setSpecularColour(DictionaryGetColour(dict, "specular"));		
	}		
}
