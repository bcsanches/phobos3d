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


#include "PH_MapLoader.h"

#include <tinyxml.h>

#include <PH_Dictionary.h>
#include <PH_DictionaryHive.h>
#include <PH_DictionaryManager.h>
#include <PH_Exception.h>
#include <PH_Kernel.h>

#define CUSTOM_PROPERTY_NODE_NAME "CUSTOMPROPERTIES"
#define PROPERTY_NODE_NAME "PROPERTY"

namespace Phobos
{
	void MapLoader_c::OnBoot()
	{
		DictionaryManagerPtr_t manager = DictionaryManager_c::GetInstance();

		ipStaticEntitiesHive = manager->CreateCustomHive("StaticEntities");
		ipDynamicEntitiesHive = manager->CreateCustomHive("DynamicEntities");
	}

	static bool ContainsCustomProperties(TiXmlElement &element)
	{
		TiXmlHandle handle(&element);

		return handle.FirstChildElement(CUSTOM_PROPERTY_NODE_NAME).FirstChildElement(PROPERTY_NODE_NAME).ToElement() ? true : false;		
	}

	static void LoadProperties(DictionaryPtr_t dict, const TiXmlElement &element)
	{
		Log_c::Stream_c stream = Kernel_c::GetInstance().LogStream();

		for(const TiXmlElement *elem = element.FirstChildElement(PROPERTY_NODE_NAME); elem; elem = elem->NextSiblingElement(PROPERTY_NODE_NAME))
		{
			const char *name = elem->Attribute("id");
			const char *value = elem->Attribute("value");

			if(name == NULL)
			{
				stream << "[MapLoader_c::LoadDictionary] id attribute not found on CUSTOMPROPERTY node\n";
				continue;
			}

			if(value == NULL)
			{
				stream << "[MapLoader_c::LoadDictionary] value attribute not found on CUSTOMPROPERTY node\n";
				continue;
			}

			dict->AddValue(name, value);
		}
	}

	static void LoadDictionary(DictionaryPtr_t dict, const TiXmlElement &element)
	{
		for(const TiXmlAttribute *atr = element.FirstAttribute(); atr; atr = atr->Next())
		{
			dict->AddValue(atr->Name(), atr->Value());			
		}		

		const TiXmlElement *custom = element.FirstChildElement(CUSTOM_PROPERTY_NODE_NAME);

		LoadProperties(dict, element);

		if(custom)
			LoadProperties(dict, *custom);
	}

	void MapLoader_c::LoadOgitor(const String_c &fileName)
	{		
		TiXmlDocument doc(fileName.c_str());	

		if(!doc.LoadFile())
		{
			PH_RAISE(FILE_NOT_FOUND_EXCEPTION, "MapLoader_c::LoadOgitor", doc.ErrorDesc());
		}	

		ipStaticEntitiesHive->RemoveAllChildren();
		ipDynamicEntitiesHive->RemoveAllChildren();
		
		TiXmlHandle root(doc.FirstChildElement());

		for(TiXmlElement *elem = root.FirstChild("OBJECT").Element();elem; elem = elem->NextSiblingElement())
		{			
			try
			{
				const char *name = elem->Attribute("name");
				if(name == NULL)
				{
					Kernel_c::GetInstance().LogMessage("[MapLoader_c::LoadOgitor] Object without name, ignored");
					continue;
				}

				//DictionaryPtr_t dict = (ContainsSpawnClass(*elem) ? ipStaticEntitiesHive : ipDynamicEntitiesHive)->CreateDictionary(
				DictionaryPtr_t dict = Dictionary_c::Create(name);

				LoadDictionary(dict, *elem);

				(ContainsCustomProperties(*elem) ? ipDynamicEntitiesHive : ipStaticEntitiesHive)->AddDictionary(dict);				
			}
			catch(Exception_c &e)
			{
				Kernel_c::GetInstance().LogMessage(e.what());
			}			
		}

		//load project data
		if(TiXmlElement *project = root.FirstChild("PROJECT").Element())
		{
			//caelum path?
			if(TiXmlElement *caelumDir = project->FirstChildElement("CAELUMDIR"))
			{
				strCaelumDir = caelumDir->Attribute("value");
			}
		}
	}
}