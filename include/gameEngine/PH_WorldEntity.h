/*
Phobos 3d
  September 2010

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

#ifndef PH_WORLD_ENTITY_H
#define PH_WORLD_ENTITY_H

#include <OgrePrerequisites.h>

#include "PH_GameEngineAPI.h"

#include "PH_Entity.h"

#define PH_WORLD_SCENE_MANAGER_NAME "SceneManager"

namespace Phobos
{
	PH_DECLARE_NODE_PTR(WorldEntity);

	class Dictionary_c;
	class MapLoader_c;
	struct TempStaticObject_s;

	class PH_GAME_ENGINE_API WorldEntity_c: public Entity_c
	{
		public:
			static EntityPtr_t Create(const String_c &name);

			void Load(const MapLoader_c &loader);

		protected:
			virtual void OnLoad(const Dictionary_c &dictionary);

		private:
			friend struct TempStaticObject_s;

			struct StaticObject_s
			{
				String_c strParent;

				Ogre::SceneNode *pclSceneNode;
				Ogre::Entity *pclEntity;
				Ogre::Light *pclLight;
			};

			WorldEntity_c(const String_c &name);
			~WorldEntity_c();

			bool LoadGlobalObject(const String_c &type, const Dictionary_c &dict);
			bool LoadStaticObject(StaticObject_s &object, const String_c &name, const String_c &type, const Dictionary_c &dict);

			void LoadEntityObject(TempStaticObject_s &temp, const Dictionary_c &dict);
			void LoadNodeObject(TempStaticObject_s &temp, const Dictionary_c &dict);
			void LoadLightObject(TempStaticObject_s &temp, const Dictionary_c &dict);

		private:
			typedef std::map<String_c, StaticObject_s> StaticObjectsMap_t;
			StaticObjectsMap_t mapStaticObjects;
	};
}

#endif
