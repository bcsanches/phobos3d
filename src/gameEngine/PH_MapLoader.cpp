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

#include <iostream>
#include <vector>

#include <rapidxml.hpp>

#include <PH_Dictionary.h>
#include <PH_DictionaryHive.h>
#include <PH_DictionaryManager.h>
#include <PH_Exception.h>
#include <PH_Kernel.h>
#include <PH_Path.h>

#include "PH_EntityFactory.h"
#include "PH_WorldEntity.h"

#define CUSTOM_PROPERTY_NODE_NAME "CUSTOMPROPERTIES"
#define PROPERTY_NODE_NAME "PROPERTY"

namespace Phobos
{	
	DictionaryHivePtr_t MapLoader_c::ipStaticEntitiesHive_g;
	DictionaryHivePtr_t MapLoader_c::ipDynamicEntitiesHive_g;
	DictionaryHivePtr_t MapLoader_c::ipCurrentLevelHive_g;

	void MapLoader_c::OnBoot()
	{
		DictionaryManagerPtr_t manager = DictionaryManager_c::GetInstance();

		ipCurrentLevelHive_g = manager->CreateCustomHive("LevelInfo");
		ipStaticEntitiesHive_g = manager->CreateCustomHive("StaticEntities");
		ipDynamicEntitiesHive_g = manager->CreateCustomHive("DynamicEntities");		
	}

	const DictionaryHive_c &MapLoader_c::GetStaticEntitiesHive() const
	{
		return *ipStaticEntitiesHive_g;
	}

	const DictionaryHive_c &MapLoader_c::GetDynamicEntitiesHive() const
	{
		return *ipDynamicEntitiesHive_g;
	}

	const DictionaryHive_c &MapLoader_c::GetCurrentLevelHive() const
	{
		return *ipCurrentLevelHive_g;
	}

	void MapLoader_c::ClearAllHives()
	{
		ipCurrentLevelHive_g->RemoveAllChildren();
		ipStaticEntitiesHive_g->RemoveAllChildren();
		ipDynamicEntitiesHive_g->RemoveAllChildren();
	}

	MapLoader_c::MapLoader_c(const Dictionary_c &settings):
		strWorldSpawnEntityType(settings.GetString("worldSpawnEntity"))
	{
		//empty
	}

	EntityPtr_t MapLoader_c::CreateAndLoadWorld()
	{		
		EntityPtr_t ptr = EntityFactory_c::GetInstance().Create(strWorldSpawnEntityType, "WorldSpawn");

		WorldEntityPtr_t world = boost::static_pointer_cast<WorldEntity_c>(ptr);
		world->Load(*this);

		return ptr;
	}
}
