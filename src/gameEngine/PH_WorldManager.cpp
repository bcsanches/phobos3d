/*
Phobos 3d
April 2010
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


#include "PH_WorldManager.h"

#include <boost/bind.hpp>

#include <PH_Console.h>
#include <PH_ContextUtils.h>
#include <PH_Dictionary.h>
#include <PH_DictionaryManager.h>
#include <PH_Error.h>
#include <PH_Exception.h>
#include <PH_Kernel.h>
#include <PH_Path.h>

#include "PH_EntityFactory.h"
#include "PH_EntityKeys.h"
#include "PH_GamePhysicsSettings.h"
#include "PH_MapLoaderFactory.h"

namespace Phobos
{
	PH_DEFINE_DEFAULT_SINGLETON(WorldManager);

	WorldManager_c::WorldManager_c():
		CoreModule_c("WorldManager", NodeFlags::PRIVATE_CHILDREN),
		cmdLoadMap("loadMap"),
		cmdUnloadMap("unloadMap"),
		cmdDumpFactoryCreators("dumpFactoryCreators")
	{
		cmdLoadMap.SetProc(PH_CONTEXT_CMD_BIND(&WorldManager_c::CmdLoadMap, this));
		cmdUnloadMap.SetProc(PH_CONTEXT_CMD_BIND(&WorldManager_c::CmdUnloadMap, this));
		cmdDumpFactoryCreators.SetProc(PH_CONTEXT_CMD_BIND(&WorldManager_c::CmdDumpFactoryCreators, this));
	}

	WorldManager_c::~WorldManager_c()
	{
		//empty
	}	

	void WorldManager_c::UnloadMap()
	{
		this->RemoveAllChildren();
		clEntityManager.Clear();

		spGameWorld.reset();

		std::for_each(lstListeners.begin(), lstListeners.end(), boost::bind(&WorldManagerListener_c::OnMapUnloaded, _1));
	}

	void WorldManager_c::LoadMap(const String_c &mapName)
	{
		this->UnloadMap();

		Path_c path(mapName);
		String_c extension;
		path.GetExtension(extension);

		spMapLoader = MapLoaderFactory_c::GetInstance().Create(extension.c_str());
		spMapLoader->Load(mapName);

		{
			auto world(spMapLoader->CreateAndLoadWorldSpawn());
			this->AddPrivateChild(std::move(world));
		}

		spGameWorld = spMapLoader->CreateAndLoadWorld();

		this->LoadEntities();

		for(Node_c::const_iterator it = this->begin(), end = this->end(); it != end; ++it)
		{
			Entity_c *entity = static_cast<Entity_c *>(it->second);

			entity->LoadFinished();
		}

		std::for_each(lstListeners.begin(), lstListeners.end(), boost::bind(&WorldManagerListener_c::OnMapLoaded, _1));
	}

	Entity_c &WorldManager_c::LoadEntity(const Dictionary_c &entityDef)
	{
		std::unique_ptr<Entity_c> ptr(EntityFactory_c::GetInstance().Create(entityDef.GetString(PH_ENTITY_KEY_CLASS_NAME), entityDef.GetName()));

		//Update handle before loading entity, so components would have a valid handle
		Handle_s h = clEntityManager.AddObject(ptr.get());
		ptr->SetHandle(h);
			
		ptr->Load(entityDef);
				
		Entity_c &entityRef = *ptr;

		this->AddPrivateChild(std::move(ptr));

		return entityRef;
	}

	void WorldManager_c::LoadEntities()
	{
		const DictionaryHive_c &hive = spMapLoader->GetDynamicEntitiesHive();		

		for(Node_c::const_iterator it = hive.begin(), end = hive.end(); it != end; ++it)
		{
			Dictionary_c *dict = static_cast<Dictionary_c *>(it->second);

			this->LoadEntity(*dict);						
		}
	}

	Entity_c *WorldManager_c::TryGetEntityByType(const String_c &className) const
	{
		for(Node_c::const_iterator it = this->begin(), end = this->end(); it != end; ++it)
		{
			Entity_c *entity = static_cast<Entity_c *>(it->second);
			if(entity->GetEntityClassName().compare(className) == 0)
				return entity;
		}

		return NULL;
	}

	Entity_c &WorldManager_c::GetEntityByName(const String_c &name) const
	{
		return static_cast<Entity_c &>(this->GetChild(name));
	}

	void WorldManager_c::OnPrepareToBoot()
	{
		Console_c &console = Console_c::GetInstance();

		console.AddContextCmd(cmdLoadMap);
		console.AddContextCmd(cmdUnloadMap);
		console.AddContextCmd(cmdDumpFactoryCreators);
	}

	void WorldManager_c::OnBoot()
	{
		MapLoader_c::OnBoot();
		GamePhysicsSettings_c::OnBoot();
	}

	void WorldManager_c::OnFinalize()
	{		
		this->UnloadMap();

		spMapLoader.reset();
	}

	void WorldManager_c::CallEntityIOProc(EntityIOList_t &list, void (EntityIO_c::*proc)())
	{
		for(EntityIOList_t::iterator it = list.begin();it != list.end();)
		{
			EntityIOList_t::iterator workIt = it++;

			(*workIt->*proc)();
		}
	}

	void WorldManager_c::OnFixedUpdate()
	{
		this->CallEntityIOProc(lstFixedUpdate, &EntityIO_c::FixedUpdate);
	}

	void WorldManager_c::OnUpdate()
	{
		this->CallEntityIOProc(lstUpdate, &EntityIO_c::Update);
	}

	void WorldManager_c::AddToFixedUpdateList(EntityIO_c &io)
	{
		lstFixedUpdate.push_back(&io);
	}

	void WorldManager_c::AddToUpdateList(EntityIO_c &io)
	{
		lstUpdate.push_back(&io);
	}

	bool WorldManager_c::RemoveFromList(EntityIOList_t &list, EntityIO_c &io)
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


	void WorldManager_c::RemoveFromFixedUpdateList(EntityIO_c &io)
	{
		PH_VERIFY_MSG(this->RemoveFromList(lstFixedUpdate, io), "Entity not in FixedUpdate list");
	}

	void WorldManager_c::RemoveFromUpdateList(EntityIO_c &io)
	{
		PH_VERIFY_MSG(this->RemoveFromList(lstUpdate, io), "Entity not in Update list");
	}

	void WorldManager_c::CmdLoadMap(const StringVector_t &args, Context_c &)
	{
		if(args.size() < 2)
		{
			Kernel_c::GetInstance().LogMessage("[CmdLoadMap] Insuficient parameters, usage: loadMap <mapName>");

			return;
		}

		try
		{
			this->LoadMap(args[1]);
		}
		catch(Exception_c &ex)
		{
			Kernel_c::GetInstance().LogMessage(ex.what());
		}
	}

	void WorldManager_c::CmdUnloadMap(const StringVector_t &args, Context_c &)
	{
		this->UnloadMap();
	}

	void WorldManager_c::CmdDumpFactoryCreators(const StringVector_t &args, Context_c &)
	{
		Log_c::Stream_c stream = Kernel_c::GetInstance().LogStream();

		stream << "Entity Factory creators:\n";

		EntityFactory_c &factory = EntityFactory_c::GetInstance();
		for(EntityFactory_c::ObjectCreatorSet_t::const_iterator it = factory.begin(), end = factory.end(); it != end; ++it)
		{
			stream << "\t" << it->GetName() << "\n";
		}

		stream << "End.";
	}

	PH_DEFINE_LISTENER_PROCS(WorldManager_c, WorldManagerListener_c, lstListeners);
}
