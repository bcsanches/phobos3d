/*
Phobos 3d
September 2011
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



#ifndef PH_TILE_WORLD_ENTITY_H
#define PH_TILE_WORLD_ENTITY_H

#include <OgrePrerequisites.h>
#include <OgreTerrainGroup.h>
#include <OgreTerrainPrerequisites.h>

#include "PH_WorldEntity.h"
#include "PH_GameEngineAPI.h"
#include "PH_PhysicsManager.h"

namespace Phobos
{
	PH_DECLARE_NODE_PTR(OgitorWorldEntity);

	class Dictionary_c;
	class MapLoader_c;
	struct TempStaticObject_s;

	class PH_GAME_ENGINE_API OgitorWorldEntity_c: public WorldEntity_c
	{
		public:
			static EntityPtr_t Create(const String_c &name);

			void Load(const MapLoader_c &loader);

		protected:
			virtual void OnLoad(const Dictionary_c &dictionary);
			virtual void OnLoadFinished();

		private:
			friend struct TempStaticObject_s;

			struct StaticObject_s
			{
				String_c strParent;

				Ogre::SceneNode *pclSceneNode;
				Ogre::Entity *pclEntity;
				Ogre::Light *pclLight;

				Physics::PhysicsManager_c::CollisionShapeSharedPtr_t spCollisionShape;
				btRigidBody *pclRigidBody;

				inline StaticObject_s():
					pclSceneNode(NULL),
					pclEntity(NULL),
					pclLight(NULL),
					pclRigidBody(NULL)
				{
					//empty
				}
			};

			OgitorWorldEntity_c(const String_c &name);
			~OgitorWorldEntity_c();

			bool LoadGlobalObject(const String_c &type, const Dictionary_c &dict);
			bool LoadStaticObject(StaticObject_s &object, const String_c &name, const String_c &type, const Dictionary_c &dict);

			void LoadEntityObject(TempStaticObject_s &temp, const Dictionary_c &dict);
			void LoadNodeObject(TempStaticObject_s &temp, const Dictionary_c &dict);
			void LoadLightObject(TempStaticObject_s &temp, const Dictionary_c &dict);

			void LoadTerrainGroup(const Dictionary_c &dict);
			void LoadTerrainPage(const Dictionary_c &dict);

			void Proc() {};

		private:
			typedef std::map<String_c, StaticObject_s> StaticObjectsMap_t;
			StaticObjectsMap_t mapStaticObjects;

			Ogre::TerrainGroup			*pclTerrainGroup;
			Ogre::TerrainGlobalOptions	*pclTerrainOptions;

			//UInt16_t					u16TerrainSize;

			const Ogre::Light			*pclTerrainLight;

			const Dictionary_c	*pclTerrainGroupDictionary;
			const Dictionary_c	*pclTerrainPageDictionary;
	};
}

#endif