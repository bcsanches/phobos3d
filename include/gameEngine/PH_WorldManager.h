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

#include <Phobos/Shell/Command.h>
#include <PH_CoreModule.h>
#include <Phobos/Listener.h>
#include <Phobos/Singleton.h>

#include "PH_Entity.h"
#include "PH_GameEngineAPI.h"
#include "PH_MapLoader.h"

namespace Phobos
{	
	PH_DECLARE_SINGLETON_PTR(WorldManager);

	typedef HandleManager<Entity> EntityManager;

	class EntityIO;	

	class WorldManagerListener
	{
		public:
			virtual ~WorldManagerListener() {};

			virtual void OnMapUnloaded() = 0;
			virtual void OnMapLoaded() = 0;

		public:
			PH_DECLARE_LISTENER_HOOK;
	};

	class PH_GAME_ENGINE_API WorldManager: public Module
	{
		PH_DECLARE_SINGLETON_METHODS(WorldManager);

		public:
			typedef std::list<EntityIO*> EntityIOList_t;

		public:
			~WorldManager();		

			void LoadMap(const String_t &mapName);
			void UnloadMap();

			Entity *TryGetEntityByType(const String_t &className) const;
			Entity &GetEntityByName(const String_t &name) const;			

			PH_DECLARE_LISTENER_PROCS(WorldManagerListener);	

			inline UInt_t GetNumActiveEntities() const;

			void AddToFixedUpdateList(EntityIO &io);
			void AddToUpdateList(EntityIO &io);

			void RemoveFromFixedUpdateList(EntityIO &io);
			void RemoveFromUpdateList(EntityIO &io);

			inline const GameWorld *GetGameWorld() const;

		protected:		
			virtual void OnBoot() override;
			virtual void OnFinalize() override;
			virtual void OnFixedUpdate() override;
			virtual void OnPrepareToBoot() override;
			virtual void OnUpdate() override;

		private:
			WorldManager();					

			void LoadEntities();
			Entity &LoadEntity(const Register::Table &entityDef);

			inline bool RemoveFromList(EntityIOList_t &list, EntityIO &io);
			inline void CallEntityIOProc(EntityIOList_t &list, void (EntityIO::*proc)());

			void CmdLoadMap(const Shell::StringVector_t &args, Shell::Context &);
			void CmdUnloadMap(const Shell::StringVector_t &args, Shell::Context &);
			void CmdDumpFactoryCreators(const Shell::StringVector_t &args, Shell::Context &);

		private:
			EntityManager m_clEntityManager;

			MapLoaderPtr_t	m_spMapLoader;

			GameWorldPtr_t	m_spGameWorld;

			Shell::Command	m_cmdLoadMap;	
			Shell::Command	m_cmdUnloadMap;	
			Shell::Command	m_cmdDumpFactoryCreators;

			PH_DECLARE_LISTENER_LIST(WorldManagerListener, m_lstListeners);

			EntityIOList_t m_lstFixedUpdate;
			EntityIOList_t m_lstUpdate;
	};

	inline UInt_t WorldManager::GetNumActiveEntities() const
	{
		return m_clEntityManager.GetNumActiveObjects();
	}

	inline const GameWorld *WorldManager::GetGameWorld() const
	{
		return m_spGameWorld.get();
	}
}

#endif