/*
Phobos 3d
April 2010
Copyright (c) 2005-2013 Bruno Sanches  http://code.google.com/p/phobos3d

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/


#include "Phobos/Game/WorldManager.h"

#include <Phobos/Engine/Console.h>
#include <Phobos/Shell/Utils.h>
#include <Phobos/Error.h>
#include <Phobos/Exception.h>
#include <Phobos/Path.h>

#include <Phobos/Register/Hive.h>
#include <Phobos/Register/Manager.h>
#include <Phobos/Register/Table.h>

#include "Phobos/Game/Level/MapDefs.h"
#include "Phobos/Game/Level/MapWorld.h"
#include "Phobos/Game/Level/MapObject.h"
#include "Phobos/Game/RegisterUtils.h"
#include "Phobos/Game/Things/EntityFactory.h"
#include "Phobos/Game/Things/Keys.h"
#include "Phobos/Game/Things/ThingsUtils.h"

#include "Phobos/Game/Physics/Settings.h"
#include "Phobos/Game/Level/MapLoaderFactory.h"

namespace Phobos
{
	namespace Game
	{
		PH_DEFINE_DEFAULT_SINGLETON2(WorldManager, Engine::Console &);

		WorldManager::WorldManager(Engine::Console &console):
			Module("WorldManager", NodeFlags::PRIVATE_CHILDREN),
			m_cmdLoadMap("loadMap"),
			m_cmdUnloadMap("unloadMap"),
			m_cmdDumpFactoryCreators("dumpFactoryCreators")
		{
			m_cmdLoadMap.SetProc(PH_CONTEXT_CMD_BIND(&WorldManager::CmdLoadMap, this));
			m_cmdUnloadMap.SetProc(PH_CONTEXT_CMD_BIND(&WorldManager::CmdUnloadMap, this));
			m_cmdDumpFactoryCreators.SetProc(PH_CONTEXT_CMD_BIND(&WorldManager::CmdDumpFactoryCreators, this));

			console.AddContextCommand(m_cmdLoadMap);
			console.AddContextCommand(m_cmdUnloadMap);
			console.AddContextCommand(m_cmdDumpFactoryCreators);
		}

		WorldManager::~WorldManager()
		{
			//empty
		}	

		void WorldManager::UnloadMap()
		{
			this->RemoveAllChildren();
			m_clEntityManager.Clear();

			//m_spGameWorld.reset();

			m_spMapLoader->Unload();
			
			m_pclMapObjectsHive->RemoveAllChildren();	

			for(auto &listener: m_lstListeners)
				listener.OnMapUnloaded();		
		}

		void WorldManager::OnMapLoaded()
		{
			for (auto &listener : m_lstListeners)
				listener.OnMapLoaded();
		}

		void WorldManager::LoadBlankMap(const char *name)
		{
			this->UnloadMap();

			this->OnMapLoaded();
		}

		void WorldManager::LoadMap(const String_t &mapName)
		{
			this->UnloadMap();

			Path path(mapName);
			String_t extension;
			path.GetExtension(extension);

			m_spMapLoader = MapLoaderFactory::GetInstance().Create(extension.c_str());
			m_spMapLoader->Load(mapName, *m_pclMapObjectsHive);

			this->LoadEntities();

			for(Node::const_iterator it = this->begin(), end = this->end(); it != end; ++it)
			{
				auto *entity = static_cast<Things::Entity *>(it->second);

				entity->LoadFinished();
			}

			this->OnMapLoaded();
		}

		Things::Entity &WorldManager::LoadEntity(const Register::Table &entityDef)
		{


			auto ptr = Things::EntityFactory::GetInstance().Create(entityDef.GetString(PH_ENTITY_KEY_CLASS_NAME), entityDef.GetName());

			//Update handle before loading entity, so components would have a valid handle
			Things::Handle_s h = m_clEntityManager.AddObject(ptr.get());
			ptr->SetHandle(h);
			
			ptr->Load(entityDef);
				
			auto &entityRef = *ptr;

			this->AddPrivateChild(std::move(ptr));

			return entityRef;
		}

		void WorldManager::LoadEntities()
		{			
			for(auto &pair : *const_cast<const Register::Hive *>(m_pclMapObjectsHive))
			{
				auto *dict = static_cast<const Register::Table *>(pair.second);

				StringRef_t type = dict->GetString(PH_MAP_OBJECT_KEY_TYPE);

				if(type.compare(PH_MAP_OBJECT_TYPE_ENTITY) == 0)
					this->LoadEntity(*dict);						
			}
		}

		Things::Entity *WorldManager::TryGetEntityByType(const String_t &className) const
		{
			for(Node::const_iterator it = this->begin(), end = this->end(); it != end; ++it)
			{
				auto *entity = static_cast<Things::Entity *>(it->second);
				if(entity->GetEntityClassName().compare(className) == 0)
					return entity;
			}

			return NULL;
		}

		Things::Entity &WorldManager::GetEntityByName(const String_t &name) const
		{
			return static_cast<Things::Entity &>(this->GetChild(name));
		}

		void WorldManager::OnInit()
		{			
			m_pclMapObjectsHive = &Register::CreateCustomHive("ObjectDef");

			Physics::Settings::Init();
		}

		void WorldManager::OnFinalize()
		{		
			this->UnloadMap();

			m_spMapLoader.reset();
		}

		void WorldManager::CallEntityIOProc(ThingList_t &list, void (Things::Thing::*proc)())
		{
			for(auto it = list.begin();it != list.end();)
			{
				auto workIt = it++;

				(*workIt->*proc)();
			}
		}

		void WorldManager::OnFixedUpdate()
		{
			this->CallEntityIOProc(m_lstFixedUpdate, &Things::Thing::FixedUpdate);
		}

		void WorldManager::OnUpdate()
		{
			this->CallEntityIOProc(m_lstUpdate, &Things::Thing::Update);
		}

		void WorldManager::AddToFixedUpdateList(Things::Thing &io)
		{
			m_lstFixedUpdate.push_back(&io);
		}

		void WorldManager::AddToUpdateList(Things::Thing &io)
		{
			m_lstUpdate.push_back(&io);
		}

		bool WorldManager::RemoveFromList(ThingList_t &list, Things::Thing &io)
		{
			for(auto it = list.begin(), end = list.end();it != end; ++it)
			{
				if(*it == &io)
				{
					list.erase(it);
					return true;
				}
			}

			return false;
		}


		void WorldManager::RemoveFromFixedUpdateList(Things::Thing &io)
		{
			PH_VERIFY_MSG(this->RemoveFromList(m_lstFixedUpdate, io), "Entity not in FixedUpdate list");
		}

		void WorldManager::RemoveFromUpdateList(Things::Thing &io)
		{
			PH_VERIFY_MSG(this->RemoveFromList(m_lstUpdate, io), "Entity not in Update list");
		}

		MapObject *WorldManager::CreateMapObject(const String_t &name, const String_t &asset, Game::MapObjectTypes type, const Engine::Math::Transform &transform)
		{
			std::unique_ptr<Register::Table> table(PH_NEW Register::Table(name));			

			if (type == MapObjectTypes::ENTITY)
			{
				table->SetString(PH_MAP_OBJECT_KEY_TYPE, PH_MAP_OBJECT_TYPE_ENTITY);
				table->SetBaseHive("EntityDef");				
			}
			else if (type == MapObjectTypes::STATIC)
			{
				table->SetString(PH_MAP_OBJECT_KEY_TYPE, PH_MAP_OBJECT_TYPE_STATIC);
				table->SetBaseHive("StaticObjectDef");				
			}

			table->SetInherited(asset);

			//it is boring to insert scale in every config, so generate a default if not set
			if (!table->HasValue(PH_MAP_OBJECT_KEY_SCALE))
			{
				Phobos::Register::SetVector3(*table, PH_MAP_OBJECT_KEY_SCALE, Ogre::Vector3::UNIT_SCALE);
			}

			Things::SaveTransform(*table, transform);					

			//Create Map object
			std::unique_ptr<MapObject> upMapObject(MapWorld::GetInstance().CreateObject(*table));			

			//
			//All is fine, commit results, transfer ownership to WorldManager and return data
			auto &refTable = *(table.get());			

			m_pclMapObjectsHive->AddTable(std::move(table));			

			//All is done, cancel the unique pointer and return a valid reference to caller
			//The object will be tracked by MapWorld
			auto pMapObject = upMapObject.release();
			return pMapObject;
		}

		void WorldManager::CmdLoadMap(const Shell::StringVector_t &args, Shell::Context &)
		{
			if(args.size() < 2)
			{
				LogMessage("[CmdLoadMap] Insuficient parameters, usage: loadMap <mapName>");

				return;
			}

			try
			{
				this->LoadMap(args[1]);
			}
			catch(Exception &ex)
			{
				LogMessage(ex.what());
			}
		}

		void WorldManager::CmdUnloadMap(const Shell::StringVector_t &args, Shell::Context &)
		{
			this->UnloadMap();
		}

		void WorldManager::CmdDumpFactoryCreators(const Shell::StringVector_t &args, Shell::Context &)
		{
			auto stream = LogMakeStream();

			stream << "Entity Factory creators:\n";

			auto &factory = Things::EntityFactory::GetInstance();
			for(auto it : factory)		
			{
				stream << "\t" << it.GetName() << "\n";
			}

			stream << "End.";
		}

		PH_DEFINE_LISTENER_PROCS(WorldManager, WorldManagerListener, m_lstListeners);
	}
}
