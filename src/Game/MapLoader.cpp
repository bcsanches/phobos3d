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


#include "Phobos/Game/MapLoader.h"

#include <iostream>
#include <vector>

#include <Phobos/Register/Manager.h>
#include <Phobos/Register/Table.h>
#include <Phobos/Register/Hive.h>

#include <Phobos/Exception.h>
#include <Phobos/Path.h>

#include "Phobos/Game/RenderWorld.h"
#include "Phobos/Game/Things/EntityFactory.h"
#include "Phobos/Game/Things/Keys.h"
#include "Phobos/Game/World.h"

#define WORLD_SPAWN_ENTITY "worldspawn"

namespace Phobos
{	
	namespace Game
	{
		Phobos::Register::Hive *MapLoader::g_pclGameObjectsHive = nullptr;
		Phobos::Register::Hive *MapLoader::g_pclCurrentLevelHive = nullptr;

		void MapLoader::OnBoot()
		{		
			g_pclCurrentLevelHive = &Register::CreateCustomHive("LevelInfo");
			g_pclGameObjectsHive = &Register::CreateCustomHive("ObjectDef");
		}	

		void MapLoader::Load(StringRef_t fileName)
		{
			ClearAllHives();

			//delegate the load processing
			this->OnLoad(fileName);			

			//Load all rendering stuff
			RenderWorldLoadAccess::Load(*g_pclGameObjectsHive);			
		}

		void MapLoader::Unload()
		{
			RenderWorldLoadAccess::Unload();
		}
		
		const Register::Hive &MapLoader::GetGameObjectsHive() const
		{
			return *g_pclGameObjectsHive;
		}

		const Register::Hive &MapLoader::GetCurrentLevelHive() const
		{
			return *g_pclCurrentLevelHive;
		}

		void MapLoader::ClearAllHives()
		{
			g_pclCurrentLevelHive->RemoveAllChildren();
			g_pclGameObjectsHive->RemoveAllChildren();			
		}

		MapLoader::MapLoader(const Register::Table &settings):
			m_strWorldSpawnEntityType(settings.GetString("worldSpawnEntityDef"))
		{
			//empty
		}

		void MapLoader::AddGameObject(std::unique_ptr<Phobos::Register::Table> obj)
		{
			g_pclGameObjectsHive->AddTable(std::move(obj));
		}

		void MapLoader::AddLevelObject(std::unique_ptr<Phobos::Register::Table> obj)
		{
			g_pclCurrentLevelHive->AddTable(std::move(obj));
		}

		std::unique_ptr<Things::Entity> MapLoader::CreateAndLoadWorldSpawn()
		{	
			Register::Table &entityDef = g_pclCurrentLevelHive->GetTable(WORLD_SPAWN_ENTITY);

			std::unique_ptr<Things::Entity> ptr(Things::EntityFactory::GetInstance().Create(entityDef.GetString(PH_ENTITY_KEY_CLASS_NAME), entityDef.GetName()));
			ptr->Load(entityDef);

			return ptr;
		}

		WorldPtr_t MapLoader::CreateAndLoadWorld()
		{
			WorldPtr_t world = this->CreateWorld();

			world->Load(*this, g_pclCurrentLevelHive->GetTable(WORLD_SPAWN_ENTITY));

			return world;
		}

		std::unique_ptr<Register::Table> MapLoader::CreateWorldSpawnEntityDef()
		{
			std::unique_ptr<Register::Table> dict(PH_NEW Register::Table(WORLD_SPAWN_ENTITY));

			dict->SetBaseHive(PH_ENTITY_DEF_HIVE);
			dict->SetInherited(m_strWorldSpawnEntityType);

			return dict;
		}
	}
}
