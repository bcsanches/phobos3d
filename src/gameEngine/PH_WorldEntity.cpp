/*
Phobos 3d
  September 2010

  Copyright (C) 2005-2010 Bruno Crivelari Sanches

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Bruno Crivelari Sanches bcsanches@gmail.com
*/

#include "PH_WorldEntity.h"

#include <boost/foreach.hpp>

#include <OgreEntity.h>
#include <OgreLight.h>
#include <OgreSceneNode.h>

#include <PH_Dictionary.h>
#include <PH_DictionaryHive.h>
#include <PH_Exception.h>
#include <PH_Kernel.h>
#include <PH_Render.h>

#include "PH_DictionaryUtils.h"
#include "PH_EntityFactory.h"
#include "PH_MapLoader.h"

#define SCENE_MANAGER_NAME "SceneManager"

namespace Phobos
{
	PH_ENTITY_CREATOR("WorldEntity", WorldEntity_c);

	struct TempStaticObject_s
	{
		Ogre::Entity *pclEntity;
		Ogre::SceneNode *pclSceneNode;
		Ogre::Light *pclLight;
		bool fParent;

		TempStaticObject_s():
			pclEntity(NULL),
			pclSceneNode(NULL),
			pclLight(NULL),
			fParent(false)
		{
		}

		inline void Commit(WorldEntity_c::StaticObject_s &object)
		{
			object.pclEntity = pclEntity;
			object.pclSceneNode = pclSceneNode;
			object.pclLight = pclLight;

			pclEntity = NULL;
			pclLight = NULL;
			pclSceneNode = NULL;
		}

		~TempStaticObject_s()
		{
			RenderPtr_t &render = Render_c::GetInstance();

			if(pclSceneNode)
				render->DestroySceneNode(pclSceneNode);

			if(pclLight)
				render->DestroyLight(pclLight);

			if(pclEntity)
				render->DestroyEntity(pclEntity);
		}
	};

	EntityPtr_t WorldEntity_c::Create(const String_c &name)
	{
		return new WorldEntity_c(name);
	}

	WorldEntity_c::WorldEntity_c(const String_c &name):
		Entity_c(name)
	{
	}

	WorldEntity_c::~WorldEntity_c()
	{
		RenderPtr_t &render = Render_c::GetInstance();
		
		BOOST_FOREACH(StaticObjectsMap_t::value_type &pair, mapStaticObjects)
		{
			StaticObject_s &object = pair.second;

			if(object.pclSceneNode)
				render->DestroySceneNode(object.pclSceneNode);

			if(object.pclLight)
				render->DestroyLight(object.pclLight);

			if(object.pclEntity)
				render->DestroyEntity(object.pclEntity);
		}
	}

	void WorldEntity_c::Load(const MapLoader_c &loader)
	{
		DictionaryHivePtr_t hive = loader.GetStaticEntitiesHive();

		for(Node_c::const_iterator it = hive->begin(), end = hive->end(); it != end; ++it)
		{
			DictionaryPtr_t dict = boost::static_pointer_cast<Dictionary_c>(it->second);

			try
			{				
				const String_c &name = dict->GetValue("name");
				const String_c &type = dict->GetValue("typename");

				if(this->LoadGlobalObject(type, *dict))
					continue;

				//make sure this is not duplicated
				StaticObjectsMap_t::iterator objIt = mapStaticObjects.lower_bound(name);
				if((objIt != mapStaticObjects.end()) && !(mapStaticObjects.key_comp()(name, objIt->first)))
				{
					Kernel_c::GetInstance().LogStream() << "[WorldEntity_c::Load] Static object " << name << " is duplicated\n";
					continue;
				}

				StaticObject_s object;
				if(!this->LoadStaticObject(object, type, *dict))
					continue;

				mapStaticObjects.insert(objIt, std::make_pair(name, object));

			}
			catch(Exception_c &ex)
			{
				Kernel_c::GetInstance().LogStream() << "[WorldEntity_c::Load] Exception loading static object: " << ex.what();
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
				Kernel_c::GetInstance().LogStream() << "[WorldEntity_c::Load] Static object " << pair.first << " without parent " << object.strParent << "\n";
				continue;
			}

			if(object.pclSceneNode->getParent())
				object.pclSceneNode->getParent()->removeChild(object.pclSceneNode);
			it->second.pclSceneNode->addChild(object.pclSceneNode);
		}
	}

	bool WorldEntity_c::LoadGlobalObject(const String_c &type, const Dictionary_c &dict)
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

		return false;
	}
	                    
	bool WorldEntity_c::LoadStaticObject(StaticObject_s &object, const String_c &type, const Dictionary_c &dict)
	{
		TempStaticObject_s temp;

		temp.fParent = dict.TryGetValue("parentnode", object.strParent) && (object.strParent.compare(SCENE_MANAGER_NAME) != 0);

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
			Kernel_c::GetInstance().LogStream() << "[WorldEntity_c::LoadStaticObject] Error, unknown static object type: " << type << "\n";
			return false;
		}		

		temp.Commit(object);
		return true;
	}

	void WorldEntity_c::LoadNodeObject(TempStaticObject_s &temp, const Dictionary_c &dict)
	{
		temp.pclSceneNode = Render_c::GetInstance()->CreateSceneNode();

		temp.pclSceneNode->setPosition(DictionaryGetVector3(dict, "position"));
		temp.pclSceneNode->setOrientation(DictionaryGetQuaternion(dict, "orientation"));
	}

	void WorldEntity_c::LoadEntityObject(TempStaticObject_s &temp, const Dictionary_c &dict)
	{
		this->LoadNodeObject(temp, dict);		

		temp.pclEntity = Render_c::GetInstance()->CreateEntity(dict.GetValue("meshfile"));
		temp.pclEntity->setCastShadows(dict.GetBool("castshadows"));
		temp.pclSceneNode->attachObject(temp.pclEntity);
	}
			
	void WorldEntity_c::LoadLightObject(TempStaticObject_s &temp, const Dictionary_c &dict)
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
					temp.pclLight->setPosition(DictionaryGetVector3(dict, "position"));
				break;

			case 1:
				temp.pclLight->setType(Ogre::Light::LT_DIRECTIONAL);
				break;

			case 2:
				{					
					temp.pclLight->setType(Ogre::Light::LT_SPOTLIGHT);

					if(!temp.fParent)
						temp.pclLight->setPosition(DictionaryGetVector3(dict, "position"));

					Ogre::Vector3 lightRange = DictionaryGetVector3(dict, "lightrange");
					temp.pclLight->setSpotlightRange(Ogre::Degree(lightRange.x), Ogre::Degree(lightRange.y), lightRange.z);
				}
				break;

			default:
				{
					std::stringstream stream;

					stream << "Invalid light type " << dict.GetInt("lighttype") << " for object " << dict.GetValue("name");
					PH_RAISE(INVALID_PARAMETER_EXCEPTION, "WorldEntity_c::LoadLightObject", stream.str());
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

	void WorldEntity_c::OnLoad(const Dictionary_c &dictionary)
	{
		//should never be called
		PH_RAISE(INVALID_OPERATION_EXCEPTION, "WorldEntity_c::OnLoad", "Not implemented");
	}
}
