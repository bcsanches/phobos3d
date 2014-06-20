/*
Phobos 3d
July 2013
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

#ifndef PH_GAME_MAP_WORLD_H
#define PH_GAME_MAP_WORLD_H

#include <Phobos/Engine/Module.h>
#include <Phobos/Singleton.h>

#include <Phobos/Register/HiveFwd.h>
#include <Phobos/Register/TableFwd.h>

#include <OgrePrerequisites.h>

#include "Phobos/Game/GameAPI.h"

namespace Phobos
{
	namespace Game
	{
		PH_DECLARE_SINGLETON_PTR(MapWorld);

		class MapObject;

		class PH_GAME_API MapWorld: public Engine::Module
		{
			PH_DECLARE_SINGLETON_METHODS(MapWorld);
			
			public:				
				class LoadAccess
				{
					friend class MapLoader;

					private:
						static void Load(StringRef_t levelPath, const Register::Hive &gameObjectsHive)
						{
							MapWorld::GetInstance().Load(levelPath, gameObjectsHive);
						}

						static void Unload()
						{
							MapWorld::GetInstance().Unload();
						}
				};
				
				//Always returns a valid pointer, that is owned by MapWorld
				//It can be destroyed by deleting it
				//The pointer will be valid as long the level is valid, if the level is unloaded, the pointer will be a dandling pointer
				virtual MapObject *CreateObject(Register::Table &table) = 0;

				//virtual void DestroyObject(MapObject *object) = 0;

			protected:
				MapWorld() : Module("MapWorld"){  };

			protected:
				virtual void Load(StringRef_t levelPath, const Register::Hive &hive) = 0;			
				virtual void Unload() = 0;							
		};

		
	}
}

#endif
