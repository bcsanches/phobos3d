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

#ifndef PH_OGITOR_GAME_WORLD_H
#define PH_OGITOR_GAME_WORLD_H

#include <Terrain/OgreTerrainGroup.h>
#include <Terrain/OgreTerrainPrerequisites.h>

#include <Phobos/Register/TableFwd.h>

#include "Phobos/Game/BaseOgreWorld.h"
#include "Phobos/Game/GameAPI.h"

namespace Phobos
{	
	namespace Game
	{
		class MapLoader;
		struct TempStaticObject_s;

		class PH_GAME_API OgitorWorld: public BaseOgreWorld
		{
			public:
				virtual void Load(const MapLoader &loader, const Register::Table &worldEntityDef);

				OgitorWorld();			
				~OgitorWorld();		

			private:
				friend struct TempStaticObject_s;			
			
				bool LoadGlobalObject(const String_t &type, const Register::Table &dict);
				bool LoadStaticObject(StaticObject_s &object, const String_t &name, const String_t &type, const Register::Table &dict);

				void LoadEntityObject(TempStaticObject_s &temp, const Register::Table &dict);
				void LoadNodeObject(TempStaticObject_s &temp, const Register::Table &dict);
				void LoadLightObject(TempStaticObject_s &temp, const Register::Table &dict);

				void LoadTerrainGroup(const Register::Table &dict);
				void LoadTerrainPage(const Register::Table &terrainPageTable, const Register::Table &worldEntityDef);

				void Proc() {};

			private:
				typedef std::map<String_t, StaticObject_s> StaticObjectsMap_t;
				StaticObjectsMap_t m_mapStaticObjects;

				Ogre::TerrainGroup			*m_pclTerrainGroup;
				Ogre::TerrainGlobalOptions	*m_pclTerrainOptions;			

				const Ogre::Light			*m_pclTerrainLight;

				const Register::Table	*m_pclTerrainGroupTable;
				const Register::Table	*m_pclTerrainPageTable;
		};
	}
}

#endif