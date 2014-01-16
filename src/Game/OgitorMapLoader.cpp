/*
Phobos 3d
September 2010
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


#include "Phobos/Game/OgitorMapLoader.h"

#include <iostream>
#include <vector>

#include <rapidxml.hpp>

#include <Phobos/Register/Table.h>
#include <Phobos/Register/Hive.h>
#include <Phobos/Register/Manager.h>

#include <Phobos/Exception.h>
#include <Phobos/Log.h>
#include <Phobos/Path.h>

#include "Phobos/Game/MapLoaderFactory.h"
#include "Phobos/Game/MapDefs.h"

#define CUSTOM_PROPERTY_NODE_NAME "CUSTOMPROPERTIES"
#define PROPERTY_NODE_NAME "PROPERTY"

namespace
{
	class ValueConversor
	{
		public:
			typedef std::function<const char *(Phobos::StringRef_t)> ConversionProc_t;

		public:
			ValueConversor(const char *key, ConversionProc_t proc = nullptr):
				m_pszKey(key),
				m_pfnConversionProc(proc)
		{
			PH_ASSERT(key);
		}

		const char *GetKey() const
		{
			return m_pszKey;
		}

		const char *Convert(Phobos::StringRef_t value) const 
		{
			return m_pfnConversionProc ? m_pfnConversionProc(value) : value.data();
		}

		private:
			const char			*m_pszKey;
			ConversionProc_t	m_pfnConversionProc;
	};

	void OutputKeyValue(Phobos::Register::Table &table, Phobos::StringRef_t key, Phobos::StringRef_t value)
	{
		static bool g_fInitialized = false;
		static std::map<Phobos::StringRef_t, ValueConversor> g_mapConversors;
		static auto g_itEndConversors = g_mapConversors.end();

		if(!g_fInitialized)
		{
			g_mapConversors.insert(std::make_pair("castshadows",	ValueConversor(PH_GAME_OBJECT_KEY_CAST_SHADOWS)));
			g_mapConversors.insert(std::make_pair("meshfile",		ValueConversor(PH_GAME_OBJECT_KEY_MESH)));
			g_mapConversors.insert(std::make_pair("lightrange",		ValueConversor(PH_GAME_OBJECT_KEY_LIGHT_RANGE)));
			g_mapConversors.insert(std::make_pair("parentnode",		ValueConversor(PH_GAME_OBJECT_KEY_PARENT_NODE, [](Phobos::StringRef_t value)
			{			
				return value.compare("SceneManager") == 0 ? nullptr : value.data();
			})));

			g_mapConversors.insert(std::make_pair("lighttype", ValueConversor(PH_GAME_OBJECT_KEY_LIGHT_TYPE, [](Phobos::StringRef_t value)
			{
				switch(std::stoi(value.data()))
				{								
					case 0:
						return PH_GAME_OBJECT_LIGHT_TYPE_POINT;										

					case 1:
						return PH_GAME_OBJECT_LIGHT_TYPE_DIRECTIONAL;					

					case 2:
						return PH_GAME_OBJECT_LIGHT_TYPE_SPOT;

					default:
						{
							std::stringstream stream;

							stream << "Invalid light type " << value;
							PH_RAISE(Phobos::INVALID_PARAMETER_EXCEPTION, "OgitorWorld::OutputKeyValue", stream.str());
						}
					break;
				}
			})));			

			g_itEndConversors = g_mapConversors.end();
			g_fInitialized = true;
		}
		
		auto it = g_mapConversors.find(key);
		if(it != g_itEndConversors)
		{
			const char *convertedValue = it->second.Convert(value.data());

			if(convertedValue)
				table.SetString(it->second.GetKey(), convertedValue);
		}
		else
		{
			table.SetString(key, value);
		}
	}
}

namespace Phobos
{
	namespace Game
	{
		PH_FULL_MAP_LOADER_CREATOR("OgitorMapLoader", OgitorMapLoader);
	
		static const char *GetChildNodeValue(const rapidxml::xml_node<> &element, const char *nodeName)
		{		
			if(rapidxml::xml_node<> *child = element.first_node(nodeName))
			{
				if(rapidxml::xml_attribute<> *valueAtr =child->first_attribute("value"))
				{
					return valueAtr->value();
				}
			}

			return NULL;
		}

		static bool IsEditorOnly(const rapidxml::xml_node<> &element)
		{
			if(rapidxml::xml_node<> *custom = element.first_node(CUSTOM_PROPERTY_NODE_NAME))
			{
				for(const rapidxml::xml_node<> *elem = custom->first_node(PROPERTY_NODE_NAME); elem; elem = elem->next_sibling(PROPERTY_NODE_NAME))
				{
					const rapidxml::xml_attribute<> *name = elem->first_attribute("id");
					if(strcmp(name->value(), "editorOnly") == 0)
					{
						const rapidxml::xml_attribute<> *value = elem->first_attribute("value");
						if(strcmp(value->value(), "true")==0)
							return true;
					}
				}
			}

			return false;
		}

		static bool ContainsCustomProperties(const rapidxml::xml_node<> &element)
		{
			if(rapidxml::xml_node<> *custom = element.first_node(CUSTOM_PROPERTY_NODE_NAME))
				return custom->first_node(PROPERTY_NODE_NAME) != NULL;
			else
				return false;
		}

		static const char *GetObjectType(const rapidxml::xml_node<> &element)
		{
			if(ContainsCustomProperties(element))
				return PH_GAME_OBJECT_TYPE_ENTITY;			

			const rapidxml::xml_attribute<> *attribute = element.first_attribute("typename");
			const char *attributeValue = attribute->value();

			static const char *g_arpszIgnoreList[] =
			{				
				"Viewport Object",
				nullptr
			};

			for(int i = 0;g_arpszIgnoreList[i]; ++i)
			{
				if(strcmp(g_arpszIgnoreList[i], attributeValue) == 0)
					return nullptr;
			}
			
			if ((strcmp(attributeValue, "Marker Object") == 0) || (strcmp(attributeValue, "Camera Object") == 0))
				return PH_GAME_OBJECT_TYPE_ENTITY;
			else if((strcmp(attributeValue, "Node Object") == 0) || (strcmp(attributeValue, "Entity Object") == 0))
				return PH_GAME_OBJECT_TYPE_STATIC;
			else if(strcmp(attributeValue, "Light Object") == 0)
				return PH_GAME_OBJECT_TYPE_STATIC_LIGHT;
			else if(strcmp(attributeValue, "OctreeSceneManager") == 0)
				return PH_GAME_OBJECT_TYPE_SCENE_MANAGER;
			else if(strcmp(attributeValue, "Terrain Group Object") == 0)
				return PH_GAME_OBJECT_TYPE_TERRAIN;
			else if(strcmp(attributeValue, "Terrain Page Object") == 0)
				return PH_GAME_OBJECT_TYPE_TERRAIN_PAGE;
			else
			{
				std::stringstream stream;
				stream << "Unknown object type: " << attributeValue;
				PH_RAISE(INVALID_PARAMETER_EXCEPTION, "OgitorMapLoader::GetObjectType", stream.str());
			}
		}

		static void LoadProperties(Register::Table &dict, const rapidxml::xml_node<> &element)
		{
			auto stream = LogMakeStream();

			for(const rapidxml::xml_node<> *elem = element.first_node(PROPERTY_NODE_NAME); elem; elem = elem->next_sibling(PROPERTY_NODE_NAME))
			{
				const rapidxml::xml_attribute<> *name = elem->first_attribute("id");
				const rapidxml::xml_attribute<> *value = elem->first_attribute("value");

				if(name == NULL)
				{
					stream << "[OgitorMapLoader::LoadProperties] id attribute not found on CUSTOMPROPERTY node\n";
					continue;
				}

				if(value == NULL)
				{
					stream << "[OgitorMapLoader::LoadProperties] value attribute not found on CUSTOMPROPERTY node\n";
					continue;
				}

				OutputKeyValue(dict, name->value(), value->value());				
			}
		}

		static void LoadTable(Register::Table &dict, const rapidxml::xml_node<> &element)
		{
			for(const rapidxml::xml_attribute<> *atr = element.first_attribute(); atr; atr = atr->next_attribute())
			{
				OutputKeyValue(dict, atr->name(), atr->value());				
			}		

			const rapidxml::xml_node<> *custom = element.first_node(CUSTOM_PROPERTY_NODE_NAME);

			LoadProperties(dict, element);

			if(custom)
				LoadProperties(dict, *custom);
		}

		OgitorMapLoader::OgitorMapLoader(const Register::Table &settings):
			MapLoader(settings)	
		{
			//empty
		}

		/*
		WorldPtr_t OgitorMapLoader::CreateWorld()
		{
			return std::make_shared<OgitorWorld>();
		}*/

		void OgitorMapLoader::OnLoad(StringRef_t fileName, Register::Hive &gameObjectsHive)
		{		
			rapidxml::xml_document<> doc;

			std::ifstream input(fileName.data(), std::ios_base::in);

			if(input.fail())			
				PH_RAISE(FILE_NOT_FOUND_EXCEPTION, "OgitorMapLoader::LoadOgitor", String_t("'") + fileName.data() + "' not found");

			std::vector<char> fileData( (std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
			fileData.push_back('\0');
		
			doc.parse<0>(&fileData[0]);	

			rapidxml::xml_node<> *root = doc.first_node();
			if(root == NULL)
				PH_RAISE(PARSER_EXCEPTION, "OgitorMapLoader::LoadOgitor", String_t("'") + fileName.data() + "' appears to be empty");		

			//std::unique_ptr<Register::Table> dict = this->CreateWorldSpawnEntityDef();

			const char *terrainDir = nullptr;

			//load project data
			if(rapidxml::xml_node<> *project = root->first_node("PROJECT"))
			{
				terrainDir = GetChildNodeValue(*project, "TERRAINDIR");				
			}													

			for(rapidxml::xml_node<> *elem = root->first_node("OBJECT");elem; elem = elem->next_sibling())
			{			
				try
				{
					rapidxml::xml_attribute<> *nameAttribute = elem->first_attribute("name");
					if(nameAttribute == NULL)
					{
						LogMessage("[OgitorMapLoader::LoadOgitor] Object without name, ignored");
						continue;
					}
					
					auto objectType = GetObjectType(*elem);
					if(!objectType)
						continue;
				
					std::unique_ptr<Register::Table> dict(PH_NEW Register::Table(nameAttribute->value()));

					dict->SetString(PH_GAME_OBJECT_KEY_TYPE, objectType);
					LoadTable(*dict, *elem);

					if(strcmp(objectType, PH_GAME_OBJECT_TYPE_STATIC_LIGHT) == 0)
					{
						dict->SetString(PH_GAME_OBJECT_KEY_ORIENTATION, "1 0 0 0");
					}
					else if(strcmp(objectType, PH_GAME_OBJECT_TYPE_TERRAIN) == 0)
					{
						dict->SetString("terrainDir", terrainDir);
					}

					if(!dict->TryGetString(PH_GAME_OBJECT_KEY_SCALE))
						dict->SetString(PH_GAME_OBJECT_KEY_SCALE, "1 1 1");

					if(strcmp(objectType, PH_GAME_OBJECT_TYPE_TERRAIN_PAGE) == 0)
					{
						auto &parent = gameObjectsHive.GetTable(dict->GetString(PH_GAME_OBJECT_KEY_PARENT_NODE));
						parent.AddSubTable(std::move(dict));
					}
					else
					{
						gameObjectsHive.AddTable(std::move(dict));
					}
				}
				catch(Exception &e)
				{
					LogMessage(e.what());
				}			
			}
		}
	}
}
