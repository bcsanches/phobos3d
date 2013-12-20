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

#include <Phobos/HandlerList.h>
#include <Phobos/Singleton.h>

#include <Phobos/Register/HiveFwd.h>

#include <OgrePrerequisites.h>

#include "Phobos/Game/GameAPI.h"

namespace Phobos
{
	namespace Game
	{
		PH_DECLARE_SINGLETON_PTR(MapWorld);

		class SceneNodeObject;

		class SceneNodeKeeper
		{
			private:
				SceneNodeKeeper(const SceneNodeKeeper &rhs);
				SceneNodeKeeper &operator=(const SceneNodeKeeper &rhs);

			public:
				~SceneNodeKeeper();
				SceneNodeKeeper();
				SceneNodeKeeper(SceneNodeObject &object, Handler h);
				SceneNodeKeeper(SceneNodeKeeper &&other);

				SceneNodeKeeper &operator=(SceneNodeKeeper &&other);

				void SetPosition(const Ogre::Vector3 &position);
				void SetOrientation(const Ogre::Quaternion &orientation);

			private:
				//The handler owns the object, not the memory
				//Must call MapWorld::DestroyDynamicnode to destroy it
				SceneNodeObject		*m_pclSceneNode;

				Handler				m_hHandler;				
		};

		class PH_GAME_API MapWorld
		{
			PH_DECLARE_SINGLETON_METHODS(MapWorld);
			
			public:
				//The returned handler will owns the object, when it dies, it destroy the object
				virtual SceneNodeKeeper AcquireDynamicSceneNodeKeeper(StringRef_t handler) = 0;

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

			protected:
				MapWorld() {};
				virtual ~MapWorld() {}

			protected:
				virtual void Load(StringRef_t levelPath, const Register::Hive &hive) = 0;			
				virtual void Unload() = 0;
		};

		
	}
}

#endif
