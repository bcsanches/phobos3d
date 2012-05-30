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

#include "PH_TileGameWorld.h"

#include <boost/foreach.hpp>

#include <OgreEntity.h>

#include <PH_Enum.h>
#include <PH_Render.h>
#include <PH_Transform.h>

#include "PH_Dictionary.h"
#include "PH_DictionaryHive.h"
#include "PH_DictionaryManager.h"
#include "PH_EntityFactory.h"
#include "PH_EntityKeys.h"
#include "PH_GameDictionaryUtils.h"
#include "PH_TileTransform.h"

namespace Phobos
{	
	TileGameWorld_c::TileGameWorld_c():
		BaseOgreGameWorld_c()
	{
		//empty
	}

	TileGameWorld_c::~TileGameWorld_c()
	{
		RenderPtr_t render = Render_c::GetInstance();

		BOOST_FOREACH(StaticObject_s &obj, vecObjects)
		{
			render->DestroySceneNode(obj.pclSceneNode);
			render->DestroyEntity(obj.pclEntity);
		}
	}

	void TileGameWorld_c::CreateMesh(int row, int col, const String_c &meshName, Float_t tileScale, const Transform_c &transform)
	{
		TempStaticObject_s obj;

		RenderPtr_t render = Render_c::GetInstance();
		obj.pclSceneNode = render->CreateSceneNode();

		obj.pclEntity = render->CreateEntity(meshName);

		obj.pclSceneNode->attachObject(obj.pclEntity);

		obj.pclSceneNode->setScale(tileScale, tileScale, tileScale);
		obj.pclSceneNode->setPosition(this->CalculatePosition(row, col));

		obj.pclSceneNode->translate(transform.GetOrigin());
		obj.pclSceneNode->rotate(transform.GetRotation());

		StaticObject_s object;
		obj.Commit(object);

		vecObjects.push_back(object);
	}

	void TileGameWorld_c::CreateCeilingMesh(int row, int col, const String_c &meshName, Float_t tileScale)
	{		
		this->CreateMesh(row, col, meshName, tileScale, Transform_c(Ogre::Vector3(0, fpTileSize, 0), Ogre::Quaternion(Ogre::Degree(180), Ogre::Vector3::UNIT_X)));
	}

	void TileGameWorld_c::CreateFloorMesh(int row, int col, const String_c &meshName, Float_t tileScale)
	{
		this->CreateMesh(row, col, meshName, tileScale, Transform_c());						
	}

	void TileGameWorld_c::CreateNorthWallMesh(int row, int col, const String_c &meshName, Float_t tileScale)
	{
		this->CreateMesh(row, col, meshName, tileScale, Transform_c(Ogre::Vector3(0, fpTileSize/2, -fpTileSize/2), Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3::UNIT_X)));
	}
	
	void TileGameWorld_c::CreateSouthWallMesh(int row, int col, const String_c &meshName, Float_t tileScale)
	{
		this->CreateMesh(row, col, meshName, tileScale, Transform_c(Ogre::Vector3(0, fpTileSize/2, fpTileSize/2), Ogre::Quaternion(Ogre::Degree(-90), Ogre::Vector3::UNIT_X)));
	}
	
	void TileGameWorld_c::CreateWestWallMesh(int row, int col, const String_c &meshName, Float_t tileScale)
	{
		this->CreateMesh(row, col, meshName, tileScale, Transform_c(Ogre::Vector3(-fpTileSize/2, fpTileSize/2, 0), Ogre::Quaternion(Ogre::Degree(-90), Ogre::Vector3::UNIT_Z)));
	}

	void TileGameWorld_c::CreateEastWallMesh(int row, int col, const String_c &meshName, Float_t tileScale)
	{
		this->CreateMesh(row, col, meshName, tileScale, Transform_c(Ogre::Vector3(fpTileSize/2, fpTileSize/2, 0), Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3::UNIT_Z)));
	}

	void TileGameWorld_c::Load(const MapLoader_c &loader, const Dictionary_c &worldEntityDictionary)
	{				
		DictionaryPtr_t tileSetDef = DictionaryManager_c::GetInstance()->GetDictionaryHive("TileSet")->GetDictionary(worldEntityDictionary.GetString("tileSet"));

		const String_c &wallMeshName = tileSetDef->GetString("wall");
		const String_c &floorMeshName = tileSetDef->GetString("floor");
		const String_c &ceilingMeshName = tileSetDef->GetString("ceiling");		
		const Float_t tileScale = tileSetDef->GetFloat("tileScale");

		fpTileSize = tileSetDef->GetFloat("tileSize");

		Dictionary_c::MatrixDataHandle_c handle = worldEntityDictionary.GetMatrix("map");

		RenderPtr_t render = Render_c::GetInstance();

		//render->SetFog(Ogre::FOG_EXP, Ogre::ColourValue::Black, 0.8f, 1, 5);		
		render->SetAmbientColor(DictionaryGetColour(worldEntityDictionary, "ambientColor"));

		for(int i = 0, numRows = handle.GetNumRows(); i < numRows; ++i)
		{
			for(int j = 0, numCols = handle.GetNumColumns(); j < numCols; ++j)
			{
				switch(handle(i, j))
				{
					case '#':
						break;

					case '.':
						this->CreateFloorMesh(i, j, floorMeshName, tileScale);
						this->CreateCeilingMesh(i, j, floorMeshName, tileScale);

						if((i > 0) && (handle(i-1, j) == '#'))
						{
							this->CreateNorthWallMesh(i, j, wallMeshName, tileScale);
						}

						if((i < handle.GetNumRows()-1) && (handle(i+1, j) == '#'))
						{
							this->CreateSouthWallMesh(i, j, wallMeshName, tileScale);
						}

						if((j > 0) && (handle(i, j-1) == '#'))
						{
							this->CreateWestWallMesh(i, j, wallMeshName, tileScale);
						}

						if((j < handle.GetNumColumns()-1) && (handle(i, j+1) == '#'))
						{
							this->CreateEastWallMesh(i, j, wallMeshName, tileScale);
						}

						break;
				}
			}
		}		
	}

	struct DirectionType_s
	{
		const char						*pstrzName;
		TileTransform_c::Direction_e	eValue;
	};

	static DirectionType_s stDirectionTypes_gl[] =
	{
		{"north", TileTransform_c::DIR_NORTH},
		{"up", TileTransform_c::DIR_NORTH},		
		{"east", TileTransform_c::DIR_EAST},
		{"right", TileTransform_c::DIR_EAST},
		{"south", TileTransform_c::DIR_SOUTH},
		{"down", TileTransform_c::DIR_SOUTH},
		{"west", TileTransform_c::DIR_WEST},
		{"left", TileTransform_c::DIR_WEST}
	};

	Ogre::Vector3 TileGameWorld_c::CalculatePosition(int row, int col) const
	{
		return Ogre::Vector3((fpTileSize * col) + fpTileSize / 2, 0, (fpTileSize * row) + fpTileSize / 2);
	}
	
	void TileGameWorld_c::LoadTileTransform(Transform_c &out, const Dictionary_c &entity) const
	{
		static Enum_c<TileTransform_c::Direction_e, DirectionType_s> clDirectionType_gl(stDirectionTypes_gl);

		const int row = entity.GetInt(PH_ENTITY_KEY_TILE_ROW);		
		const int col = entity.GetInt(PH_ENTITY_KEY_TILE_COL);

		const String_c &direction = entity.GetString("faces");

		TileTransform_c::Direction_e dir;
		if(!clDirectionType_gl.TryGetValue(dir, direction))
		{
			std::stringstream stream;
			stream << "Invalid value for faces parameters: " << direction;

			PH_RAISE(INVALID_PARAMETER_EXCEPTION, "TileGameWorld_c::LoadTileTransform", stream.str());
		}

		out.SetOrigin(this->CalculatePosition(row, col));
		out.SetRotation(Ogre::Quaternion(Ogre::Degree(90.0f * dir), Ogre::Vector3::UNIT_Y));		
	}
}
