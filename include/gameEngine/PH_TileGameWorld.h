/*
Phobos 3d
May 2012
Copyright (c) 2005-2012 Bruno Sanches  http://code.google.com/p/phobos3d

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/


#ifndef PH_TILE_GAME_WORLD_H
#define PH_TILE_GAME_WORLD_H

#include "PH_BaseOgreGameWorld.h"

#include <vector>

namespace Phobos
{	
	class Transform_c;

	class PH_GAME_ENGINE_API TileGameWorld_c: public BaseOgreGameWorld_c
	{
		public:			
			TileGameWorld_c();
			~TileGameWorld_c();

			virtual void Load(const MapLoader_c &loader, const Dictionary_c &worldEntityDictionary);

			void LoadTileTransform(Transform_c &out, const Dictionary_c &entity) const;

			Ogre::Vector3 CalculatePosition(int row, int col) const;
		
		private:			
			void CreateFloorMesh(int row, int col, const String_c &meshName, Float_t tileScale);
			void CreateCeilingMesh(int row, int col, const String_c &meshName, Float_t tileScale);

			void CreateNorthWallMesh(int row, int col, const String_c &meshName, Float_t tileScale);
			void CreateSouthWallMesh(int row, int col, const String_c &meshName, Float_t tileScale);
			void CreateWestWallMesh(int row, int col, const String_c &meshName, Float_t tileScale);
			void CreateEastWallMesh(int row, int col, const String_c &meshName, Float_t tileScale);

			void CreateMesh(int row, int col, const String_c &meshName, Float_t tileScale, const Transform_c &transform);

		private:
			std::vector<StaticObject_s> vecObjects;

			Float_t fpTileSize;			
	};
}

#endif