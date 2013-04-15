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


#include "PH_WorldManager.h"

#include <Phobos/Engine/Console.h>
#include <Phobos/Shell/Utils.h>
#include <Phobos/Error.h>
#include <Phobos/Exception.h>
#include <Phobos/Path.h>

#include <Phobos/Register/Hive.h>
#include <Phobos/Register/Manager.h>
#include <Phobos/Register/Table.h>

#include "PH_EntityFactory.h"
#include "PH_EntityKeys.h"
#include "PH_GamePhysicsSettings.h"
#include "PH_MapLoaderFactory.h"

namespace Phobos
{
	PH_DEFINE_DEFAULT_SINGLETON(WorldManager);

	WorldManager::WorldManager():
		Module("WorldManager", NodeFlags::PRIVATE_CHILDREN),
		m_cmdLoadMap("loadMap"),
		m_cmdUnloadMap("unloadMap"),
		m_cmdDumpFactoryCreators("dumpFactoryCreators")
	{
		m_cmdLoadMap.SetProc(PH_CONTEXT_CMD_BIND(&WorldManager::CmdLoadMap, this));
		m_cmdUnloadMap.SetProc(PH_CONTEXT_CMD_BIND(&WorldManager::CmdUnloadMap, this));
		m_cmdDumpFactoryCreators.SetProc(PH_CONTEXT_CMD_BIND(&WorldManager::CmdDumpFactoryCreators, this));
	}

	WorldManager::~WorldManager()
	{
		//empty
	}	

	void WorldManager::UnloadMap()
	{
		this->RemoveAllChildren();
		m_clEntityManager.Clear();

		m_spGameWorld.reset();

		for(auto &listener: m_lstListeners)
			listener.OnMapUnloaded();		
	}

	void WorldManager::LoadMap(const String_t &mapName)
	{
		this->UnloadMap();

		Path path(mapName);
		String_t extension;
		path.GetExtension(extension);

		m_spMapLoader = MapLoaderFactory::GetInstance().Create(extension.c_str());
		m_spMapLoader->Load(mapName);

		{
			auto world(m_spMapLoader->CreateAndLoadWorldSpawn());
			this->AddPrivateChild(std::move(world));
		}

		m_spGameWorld = m_spMapLoader->CreateAndLoadWorld();

		this->LoadEntities();

		for(Node::const_iterator it = this->begin(), end = this->end(); it != end; ++it)
		{
			Entity *entity = static_cast<Entity *>(it->second);

			entity->LoadFinished();
		}

		for(auto &listener: m_lstListeners)
			listener.OnMapLoaded();		
	}

	Entity &WorldManager::LoadEntity(const Register::Table &entityDef)
	{
		std::unique_ptr<Entity> ptr(EntityFactory::GetInstance().Create(entityDef.GetString(PH_ENTITY_KEY_CLASS_NAME), entityDef.GetName()));

		//Update handle before loading entity, so components would have a valid handle
		Handle_s h = m_clEntityManager.AddObject(ptr.get());
		ptr->SetHandle(h);
			
		ptr->Load(entityDef);
				
		Entity &entityRef = *ptr;

		this->AddPrivateChild(std::move(ptr));

		return entityRef;
	}

	void WorldManager::LoadEntities()
	{
		auto &hive = m_spMapLoader->GetDynamicEntitiesHive();		

		for(Node::const_iterator it = hive.begin(), end = hive.end(); it != end; ++it)
		{
			auto *dict = static_cast<Register::Table *>(it->second);

			this->LoadEntity(*dict);						
		}
	}

	Entity *WorldManager::TryGetEntityByType(const String_t &className) const
	{
		for(Node::const_iterator it = this->begin(), end = this->end(); it != end; ++it)
		{
			Entity *entity = static_cast<Entity *>(it->second);
			if(entity->GetEntityClassName().compare(className) == 0)
				return entity;
		}

		return NULL;
	}

	Entity &WorldManager::GetEntityByName(const String_t &name) const
	{
		return static_cast<Entity &>(this->GetChild(name));
	}

	void WorldManager::OnPrepareToBoot()
	{
		auto &console = Engine::Console::GetInstance();

		console.AddContextCommand(m_cmdLoadMap);
		console.AddContextCommand(m_cmdUnloadMap);
		console.AddContextCommand(m_cmdDumpFactoryCreators);
	}

	void WorldManager::OnBoot()
	{
		MapLoader::OnBoot();
		GamePhysicsSettings::OnBoot();
	}

	void WorldManager::OnFinalize()
	{		
		this->UnloadMap();

		m_spMapLoader.reset();
	}

	void WorldManager::CallEntityIOProc(EntityIOList_t &list, void (EntityIO::*proc)())
	{
		for(auto it = list.begin();it != list.end();)
		{
			auto workIt = it++;

			(*workIt->*proc)();
		}
	}

	void WorldManager::OnFixedUpdate()
	{
		this->CallEntityIOProc(m_lstFixedUpdate, &EntityIO::FixedUpdate);
	}

	void WorldManager::OnUpdate()
	{
		this->CallEntityIOProc(m_lstUpdate, &EntityIO::Update);
	}

	void WorldManager::AddToFixedUpdateList(EntityIO &io)
	{
		m_lstFixedUpdate.push_back(&io);
	}

	void WorldManager::AddToUpdateList(EntityIO &io)
	{
		m_lstUpdate.push_back(&io);
	}

	bool WorldManager::RemoveFromList(EntityIOList_t &list, EntityIO &io)
	{
		for(EntityIOList_t::iterator it = list.begin(), end = list.end();it != end; ++it)
		{
			if(*it == &io)
			{
				list.erase(it);
				return true;
			}
		}

		return false;
	}


	void WorldManager::RemoveFromFixedUpdateList(EntityIO &io)
	{
		PH_VERIFY_MSG(this->RemoveFromList(m_lstFixedUpdate, io), "Entity not in FixedUpdate list");
	}

	void WorldManager::RemoveFromUpdateList(EntityIO &io)
	{
		PH_VERIFY_MSG(this->RemoveFromList(m_lstUpdate, io), "Entity not in Update list");
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

		EntityFactory &factory = EntityFactory::GetInstance();
		for(auto it : factory)		
		{
			stream << "\t" << it.GetName() << "\n";
		}

		stream << "End.";
	}

	PH_DEFINE_LISTENER_PROCS(WorldManager, WorldManagerListener, m_lstListeners);
}
