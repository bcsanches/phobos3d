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
#include "PH_EntityKeys.h"
#include "PH_GameWorld.h"

#define WORLD_SPAWN_ENTITY "worldspawn"

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
		strWorldSpawnEntityType(settings.GetString("worldSpawnEntityDef"))
	{
		//empty
	}

	EntityPtr_t MapLoader_c::CreateAndLoadWorldSpawn()
	{	
		DictionaryPtr_t entityDef = ipCurrentLevelHive_g->GetDictionary(WORLD_SPAWN_ENTITY);
		EntityPtr_t ptr = EntityFactory_c::GetInstance().Create(entityDef->GetString(PH_ENTITY_KEY_CLASS_NAME), entityDef->GetName());

		ptr->Load(*entityDef);

		return ptr;
	}

	GameWorldPtr_t MapLoader_c::CreateAndLoadWorld()
	{
		GameWorldPtr_t world = this->CreateGameWorld();

		world->Load(*this, *(ipCurrentLevelHive_g->GetDictionary(WORLD_SPAWN_ENTITY)));

		return world;
	}

	DictionaryPtr_t MapLoader_c::CreateWorldSpawnEntityDictionary()
	{
		DictionaryPtr_t dict = Dictionary_c::Create(WORLD_SPAWN_ENTITY);

		dict->SetBaseHive("EntityDef");
		dict->SetInherited(strWorldSpawnEntityType);

		return dict;
	}
}
