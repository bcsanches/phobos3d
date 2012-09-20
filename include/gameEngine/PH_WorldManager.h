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

#include <PH_ContextCmd.h>
#include <PH_CoreModule.h>
#include <PH_Listener.h>
#include <PH_Singleton.h>

#include "PH_Entity.h"
#include "PH_GameEngineAPI.h"
#include "PH_MapLoader.h"

namespace Phobos
{	
	PH_DECLARE_SINGLETON_PTR(WorldManager);

	typedef HandleManager_c<Entity_c> EntityManager_c;

	class EntityIO_c;	

	class WorldManagerListener_c
	{
		public:
			virtual ~WorldManagerListener_c() {};

			virtual void OnMapUnloaded() = 0;
			virtual void OnMapLoaded() = 0;

		public:
			PH_DECLARE_LISTENER_HOOK;
	};

	class PH_GAME_ENGINE_API WorldManager_c: public CoreModule_c
	{
		PH_DECLARE_SINGLETON_METHODS(WorldManager);

		public:
			typedef std::list<EntityIO_c*> EntityIOList_t;

		public:
			~WorldManager_c();		

			void LoadMap(const String_c &mapName);
			void UnloadMap();

			Entity_c *TryGetEntityByType(const String_c &className) const;
			Entity_c &GetEntityByName(const String_c &name) const;			

			PH_DECLARE_LISTENER_PROCS(WorldManagerListener_c);	

			inline UInt_t GetNumActiveEntities() const;

			void AddToFixedUpdateList(EntityIO_c &io);
			void AddToUpdateList(EntityIO_c &io);

			void RemoveFromFixedUpdateList(EntityIO_c &io);
			void RemoveFromUpdateList(EntityIO_c &io);

			inline const GameWorld_c *GetGameWorld() const;

		protected:		
			virtual void OnBoot();
			virtual void OnFinalize();
			virtual void OnFixedUpdate();
			virtual void OnPrepareToBoot();
			virtual void OnUpdate();

		private:
			WorldManager_c();					

			void LoadEntities();
			Entity_c &LoadEntity(const Dictionary_c &entityDef);

			inline bool RemoveFromList(EntityIOList_t &list, EntityIO_c &io);
			inline void CallEntityIOProc(EntityIOList_t &list, void (EntityIO_c::*proc)());

			void CmdLoadMap(const StringVector_t &args, Context_c &);
			void CmdUnloadMap(const StringVector_t &args, Context_c &);
			void CmdDumpFactoryCreators(const StringVector_t &args, Context_c &);

		private:
			EntityManager_c clEntityManager;

			MapLoaderPtr_t	spMapLoader;

			GameWorldPtr_t	spGameWorld;

			ContextCmd_c	cmdLoadMap;	
			ContextCmd_c	cmdUnloadMap;	
			ContextCmd_c	cmdDumpFactoryCreators;

			PH_DECLARE_LISTENER_LIST(WorldManagerListener_c, lstListeners);

			EntityIOList_t lstFixedUpdate;
			EntityIOList_t lstUpdate;
	};

	inline UInt_t WorldManager_c::GetNumActiveEntities() const
	{
		return clEntityManager.GetNumActiveObjects();
	}

	inline const GameWorld_c *WorldManager_c::GetGameWorld() const
	{
		return spGameWorld.get();
	}
}

#endif