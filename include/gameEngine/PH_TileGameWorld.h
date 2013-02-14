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

#include "PH_CollisionTagFwd.h"

#include <Phobos/Register/TableFwd.h>

#include <vector>

namespace Phobos
{	
	class Transform;
	class TileTransform;

	class PH_GAME_ENGINE_API TileGameWorld: public BaseOgreGameWorld
	{
		public:			
			TileGameWorld();
			~TileGameWorld();

			virtual void Load(const MapLoader &loader, const Register::Table &worldEntityTable);

			TileTransform CreateTileTransform(const Register::Table &entity) const;
			void LoadTileTransform(TileTransform &out, const Register::Table &entity) const;
			void LoadTileTransform(Transform &out, const Register::Table &entity) const;
			void TileTransform2Transform(Transform &out, const TileTransform &tileTransform) const;

			Ogre::Vector3 CalculatePosition(int row, int col) const;
		
		private:			
			void CreateFloorMesh(int row, int col, const String_t &meshName, Float_t tileScale, const String_t *optionalMaterial, const Physics::CollisionTag &collisionTag);
			void CreateCeilingMesh(int row, int col, const String_t &meshName, Float_t tileScale, const String_t *optionalMaterial, const Physics::CollisionTag &collisionTag);

			void CreateNorthWallMesh(int row, int col, const String_t &meshName, Float_t tileScale, const String_t *optionalMaterial, const Physics::CollisionTag &collisionTag);
			void CreateSouthWallMesh(int row, int col, const String_t &meshName, Float_t tileScale, const String_t *optionalMaterial, const Physics::CollisionTag &collisionTag);
			void CreateWestWallMesh(int row, int col, const String_t &meshName, Float_t tileScale, const String_t *optionalMaterial, const Physics::CollisionTag &collisionTag);
			void CreateEastWallMesh(int row, int col, const String_t &meshName, Float_t tileScale, const String_t *optionalMaterial, const Physics::CollisionTag &collisionTag);

			void SpawnMesh(int row, int col, const String_t &meshName, Float_t tileScale, const Transform &transform, const String_t *optionalMaterial, const Physics::CollisionTag &collisionTag);
			void SpawnMesh(const TileTransform transform, const String_t &meshName, const Ogre::Vector3 &scale, const String_t *optionalMaterial, const Physics::CollisionTag &collisionTag);

			void CreateStaticObjectNode(TempStaticObject_s &obj, const TileTransform &tileTransform, const Ogre::Vector3 &scale);
			void CreateStaticObjectNode(TempStaticObject_s &obj, const Register::Table &dict, const Ogre::Vector3 &scale);
			void CreateStaticObjectMesh(TempStaticObject_s &obj, const String_t &meshName, const String_t *optionalMaterial) const;

			void CreateStaticObjectRigidBody(StaticObject_s &obj, const Ogre::Vector3 &scale, const Physics::CollisionTag &collisionTag);

			StaticObject_s &CommitTempObject(TempStaticObject_s &obj);

		private:
			std::vector<StaticObject_s> m_vecObjects;

			Float_t m_fpTileSize;
	};
}

#endif