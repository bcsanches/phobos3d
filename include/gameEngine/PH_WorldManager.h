/*
Phobos 3d
  April 2010

  Copyright (C) 2005-2010 Bruno Crivelari Sanches

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Bruno Crivelari Sanches bcsanches@gmail.com
*/

#ifndef PH_WORLD_MANAGER_H
#define PH_WORLD_MANAGER_H

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
			void LoadMap(const String_c &mapName);

			EntityPtr_t TryGetEntityByType(const String_c &className);

			PH_DECLARE_LISTENER_PROCS(WorldManagerListener_c);

		protected:		
			void OnPrepareToBoot();
			void OnBoot();
			void OnFinalize();

		private:
			WorldManager_c();
			~WorldManager_c();				

			void LoadEntities();

			void CmdLoadMap(const StringVector_t &args, Context_c &);

		private:
			MapLoader_c		clMapLoader;

			ContextCmd_c	cmdLoadMap;	

			PH_DECLARE_LISTENER_LIST(WorldManagerListener_c, lstListeners);			
	};
}

#endif