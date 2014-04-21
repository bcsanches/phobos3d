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


#ifndef PH_WORLD_MANAGER_H
#define PH_WORLD_MANAGER_H

#include <list>
#include <tuple>

#include <Phobos/Engine/ConsoleFwd.h>
#include <Phobos/Engine/Module.h>
#include <Phobos/HandleList.h>
#include <Phobos/Listener.h>
#include <Phobos/OgreEngine/Math/TransformFwd.h>
#include <Phobos/Shell/Command.h>
#include <Phobos/Singleton.h>
#include <Phobos/Register/HiveFwd.h>
#include <Phobos/Register/TableFwd.h>


#include "Phobos/Game/Things/Entity.h"
#include "Phobos/Game/GameAPI.h"
#include "Phobos/Game/MapDefs.h"
#include "Phobos/Game/MapLoader.h"
#include "Phobos/Game/Things/ThingFwd.h"

namespace Phobos
{	
	namespace Game
	{
		PH_DECLARE_SINGLETON_PTR(WorldManager);

		typedef Things::HandleManager<Things::Entity> EntityManager;		

		class WorldManagerListener
		{
			public:
				virtual ~WorldManagerListener() {};

				virtual void OnMapUnloaded() = 0;
				virtual void OnMapLoaded() = 0;

			public:
				PH_DECLARE_LISTENER_HOOK;
		};

		class PH_GAME_API WorldManager: public Phobos::Engine::Module
		{
			PH_DECLARE_SINGLETON_METHODS2(WorldManager, Engine::Console &);

			public:
				typedef std::list<Things::Thing*> ThingList_t;

			public:					
				void LoadBlankMap(const char *name);
				void LoadMap(const String_t &mapName);
				void UnloadMap();

				Things::Entity *TryGetEntityByType(const String_t &className) const;
				Things::Entity &GetEntityByName(const String_t &name) const;			

				PH_DECLARE_LISTENER_PROCS(WorldManagerListener);	

				inline UInt_t GetNumActiveEntities() const;

				void AddToFixedUpdateList(Things::Thing &io);
				void AddToUpdateList(Things::Thing &io);

				void RemoveFromFixedUpdateList(Things::Thing &io);
				void RemoveFromUpdateList(Things::Thing &io);

				std::tuple<Register::Table &, Handle> MakeMapObject(const String_t &name, const String_t &asset, Game::MapObjectTypes type, const Engine::Math::Transform &transform);

			protected:		
				virtual void OnInit() override;
				virtual void OnFinalize() override;
				virtual void OnFixedUpdate() override;
				virtual void OnUpdate() override;

			private:
				WorldManager(Engine::Console &console);
				virtual ~WorldManager();

				void LoadEntities();
				Things::Entity &LoadEntity(const Phobos::Register::Table &entityDef);

				inline bool RemoveFromList(ThingList_t &list, Things::Thing &io);
				inline void CallEntityIOProc(ThingList_t &list, void (Things::Thing::*proc)());

				void OnMapLoaded();

				void CmdLoadMap(const Shell::StringVector_t &args, Shell::Context &);
				void CmdUnloadMap(const Shell::StringVector_t &args, Shell::Context &);
				void CmdDumpFactoryCreators(const Shell::StringVector_t &args, Shell::Context &);

			private:
				EntityManager	m_clEntityManager;

				MapLoaderPtr_t	m_spMapLoader;

				Shell::Command	m_cmdLoadMap;	
				Shell::Command	m_cmdUnloadMap;	
				Shell::Command	m_cmdDumpFactoryCreators;

				Phobos::Register::Hive *m_pclMapObjectsHive;				

				PH_DECLARE_LISTENER_LIST(WorldManagerListener, m_lstListeners);

				ThingList_t m_lstFixedUpdate;
				ThingList_t m_lstUpdate;
		};

		inline UInt_t WorldManager::GetNumActiveEntities() const
		{
			return m_clEntityManager.GetNumActiveObjects();
		}
	}
}

#endif