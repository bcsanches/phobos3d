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
#include <PH_Kernel.h>
#include <PH_Render.h>
#include <PH_Transform.h>

#include <Phobos/Register/Table.h>
#include <Phobos/Register/Hive.h>
#include <Phobos/Register/Manager.h>

#include "PH_EntityFactory.h"
#include "PH_EntityKeys.h"
#include "PH_GameRegisterUtils.h"
#include "PH_GamePhysicsSettings.h"
#include "PH_MapLoader.h"
#include "PH_PhysicsManager.h"
#include "PH_RigidBody.h"
#include "PH_TileTransform.h"

namespace Phobos
{	
	static inline bool CheckNorthTileEqual(const Register::Table_c::MatrixDataHandle_c &handle, int row, int col, char tile)
	{
		return (row > 0) && handle(row-1, col) == tile;
	}

	static inline bool CheckWestTileEqual(const Register::Table_c::MatrixDataHandle_c &handle, int row, int col, char tile)
	{
		return (col > 0) && handle(row, col-1) == tile;
	}

	static inline bool CheckSouthTileEqual(const Register::Table_c::MatrixDataHandle_c &handle, int row, int col, char tile)
	{
		return (row < handle.GetNumRows() - 1) && handle(row+1, col) == tile;
	}		

	static inline bool CheckEastTileEqual(const Register::Table_c::MatrixDataHandle_c &handle, int row, int col, char tile)
	{
		return (col < handle.GetNumColumns()-1) && handle(row, col+1) == tile;
	}

	static inline bool HasNorthWall(const Register::Table_c::MatrixDataHandle_c &handle, int row, int col)
	{
		return CheckNorthTileEqual(handle, row, col, '#');
	}

	static inline bool HasWestWall(const Register::Table_c::MatrixDataHandle_c &handle, int row, int col)
	{
		return CheckWestTileEqual(handle, row, col, '#');
	}

	static inline bool HasSouthWall(const Register::Table_c::MatrixDataHandle_c &handle, int row, int col)
	{
		return CheckSouthTileEqual(handle, row, col, '#');
	}	

	static inline bool HasEastWall(const Register::Table_c::MatrixDataHandle_c &handle, int row, int col)
	{
		return CheckEastTileEqual(handle, row, col, '#');
	}

	static inline bool IsNorthFloor(const Register::Table_c::MatrixDataHandle_c &handle, int row, int col)
	{
		return CheckNorthTileEqual(handle, row, col, '.');
	}

	static inline bool IsWestFloor(const Register::Table_c::MatrixDataHandle_c &handle, int row, int col)
	{
		return CheckWestTileEqual(handle, row, col, '.');
	}

	static inline bool IsSouthFloor(const Register::Table_c::MatrixDataHandle_c &handle, int row, int col)
	{
		return CheckSouthTileEqual(handle, row, col, '.');
	}	

	static inline bool IsEastFloor(const Register::Table_c::MatrixDataHandle_c &handle, int row, int col)
	{
		return CheckEastTileEqual(handle, row, col, '.');
	}

	TileGameWorld_c::TileGameWorld_c():
		BaseOgreGameWorld_c()
	{
		//empty
	}

	TileGameWorld_c::~TileGameWorld_c()
	{		
		BOOST_FOREACH(StaticObject_s &obj, vecObjects)
		{
			obj.Clear();
		}
	}

	void TileGameWorld_c::CreateStaticObjectRigidBody(StaticObject_s &staticObj, const Ogre::Vector3 &scale, const Physics::CollisionTag_c &collisionTag)
	{
		Transform_c transform(staticObj.pclSceneNode->_getDerivedPosition(), staticObj.pclSceneNode->_getDerivedOrientation());

		BaseOgreGameWorld_c::CreateStaticObjectRigidBody(staticObj, transform, scale, collisionTag);		
	}

	BaseOgreGameWorld_c::StaticObject_s &TileGameWorld_c::CommitTempObject(TempStaticObject_s &obj)
	{
		StaticObject_s object;
		obj.Commit(object);

		vecObjects.push_back(object);		

		return vecObjects.back();
	}

	void TileGameWorld_c::CreateStaticObjectNode(TempStaticObject_s &obj, const TileTransform_c &tileTransform, const Ogre::Vector3 &scale)
	{
		obj.pclSceneNode = Render_c::GetInstance().CreateSceneNode();

		Transform_c transform;
		this->TileTransform2Transform(transform, tileTransform);

		obj.pclSceneNode->setPosition(transform.GetOrigin());
		obj.pclSceneNode->setOrientation(transform.GetRotation());
		obj.pclSceneNode->setScale(scale);
	}

	void TileGameWorld_c::CreateStaticObjectNode(TempStaticObject_s &obj, const Register::Table_c &table, const Ogre::Vector3 &scale)
	{		
		this->CreateStaticObjectNode(obj, this->CreateTileTransform(table), scale);		
	}

	void TileGameWorld_c::CreateStaticObjectMesh(TempStaticObject_s &obj, const String_t &meshName, const String_t *optionalMaterial) const
	{
		obj.pclEntity = Render_c::GetInstance().CreateEntity(meshName);
		obj.pclEntity->setCastShadows(true);		

		if(optionalMaterial)
			obj.pclEntity->setMaterialName(*optionalMaterial);

		obj.pclSceneNode->attachObject(obj.pclEntity);
	}

	void TileGameWorld_c::SpawnMesh(int row, int col, const String_t &meshName, Float_t tileScale, const Transform_c &transform, const String_t *optionalMaterial, const Physics::CollisionTag_c &collisionTag)
	{
		TempStaticObject_s obj;

		Render_c &render = Render_c::GetInstance();
		obj.pclSceneNode = render.CreateSceneNode();

		this->CreateStaticObjectMesh(obj, meshName, optionalMaterial);

		obj.pclSceneNode->setScale(tileScale, tileScale, tileScale);
		obj.pclSceneNode->setPosition(this->CalculatePosition(row, col));

		obj.pclSceneNode->translate(transform.GetOrigin());
		obj.pclSceneNode->rotate(transform.GetRotation());		

		this->CreateStaticObjectRigidBody(this->CommitTempObject(obj), Ogre::Vector3(tileScale, tileScale, tileScale), collisionTag);
	}
	
	void TileGameWorld_c::SpawnMesh(const TileTransform_c tileTransform, const String_t &meshName, const Ogre::Vector3 &scale, const String_t *optionalMaterial, const Physics::CollisionTag_c &collisionTag)
	{
		TempStaticObject_s obj;							

		this->CreateStaticObjectNode(obj, tileTransform, scale);
		this->CreateStaticObjectMesh(obj, meshName, optionalMaterial);

		this->CreateStaticObjectRigidBody(this->CommitTempObject(obj), scale, collisionTag);
	}

	void TileGameWorld_c::CreateCeilingMesh(int row, int col, const String_t &meshName, Float_t tileScale, const String_t *optionalMaterial, const Physics::CollisionTag_c &collisionTag)
	{		
		this->SpawnMesh(row, col, meshName, tileScale, Transform_c(Ogre::Vector3(0, fpTileSize, 0), Ogre::Quaternion(Ogre::Degree(180), Ogre::Vector3::UNIT_X)), optionalMaterial, collisionTag);
	}

	void TileGameWorld_c::CreateFloorMesh(int row, int col, const String_t &meshName, Float_t tileScale, const String_t *optionalMaterial, const Physics::CollisionTag_c &collisionTag)
	{
		this->SpawnMesh(row, col, meshName, tileScale, Transform_c(), optionalMaterial, collisionTag);						
	}

	void TileGameWorld_c::CreateNorthWallMesh(int row, int col, const String_t &meshName, Float_t tileScale, const String_t *optionalMaterial, const Physics::CollisionTag_c &collisionTag)
	{
		this->SpawnMesh(row, col, meshName, tileScale, Transform_c(Ogre::Vector3(0, fpTileSize/2, -fpTileSize/2), Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3::UNIT_X)), optionalMaterial, collisionTag);
	}
	
	void TileGameWorld_c::CreateSouthWallMesh(int row, int col, const String_t &meshName, Float_t tileScale, const String_t *optionalMaterial, const Physics::CollisionTag_c &collisionTag)
	{
		this->SpawnMesh(
			row, 
			col, 
			meshName, 
			tileScale, 
			Transform_c(
				Ogre::Vector3(0, fpTileSize/2, fpTileSize/2), Ogre::Quaternion(Ogre::Degree(-90), Ogre::Vector3::UNIT_X) * Ogre::Quaternion(Ogre::Degree(180), Ogre::Vector3::UNIT_Y)
			), 
			optionalMaterial,
			collisionTag
		);
	}
	
	void TileGameWorld_c::CreateWestWallMesh(int row, int col, const String_t &meshName, Float_t tileScale, const String_t *optionalMaterial, const Physics::CollisionTag_c &collisionTag)
	{
		this->SpawnMesh(
			row, 
			col, 
			meshName, 
			tileScale, 
			Transform_c(
				Ogre::Vector3(-fpTileSize/2, fpTileSize/2, 0), 
				Ogre::Quaternion(Ogre::Degree(-90), Ogre::Vector3::UNIT_Z) * Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3::UNIT_Y)
			), 
			optionalMaterial,
			collisionTag
		);
	}

	void TileGameWorld_c::CreateEastWallMesh(int row, int col, const String_t &meshName, Float_t tileScale, const String_t *optionalMaterial, const Physics::CollisionTag_c &collisionTag)
	{
		this->SpawnMesh(
			row, 
			col, 
			meshName, 
			tileScale, 
			Transform_c(
				Ogre::Vector3(fpTileSize/2, fpTileSize/2, 0), 
				Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3::UNIT_Z) * Ogre::Quaternion(Ogre::Degree(-90), Ogre::Vector3::UNIT_Y)
			),
			optionalMaterial,
			collisionTag
		);
	}

	void TileGameWorld_c::Load(const MapLoader_c &loader, const Register::Table_c &worldEntityTable)
	{				
		auto &tileSetDef = Register::GetHive("TileSet").GetTable(worldEntityTable.GetString("tileSet"));

		const String_t &wallMeshName = tileSetDef.GetString("wall");
		const String_t *wallMaterial = tileSetDef.TryGetString("wallMaterial");

		const String_t &floorMeshName = tileSetDef.GetString("floor");
		const String_t *floorMaterial = tileSetDef.TryGetString("floorMaterial");

		const String_t &ceilingMeshName = tileSetDef.GetString("ceiling");		
		const String_t *ceilingMaterial = tileSetDef.TryGetString("ceilingMaterial");

		const Float_t tileScale = tileSetDef.GetFloat("tileScale");

		bool supressCeiling = false;
		worldEntityTable.TryGetBool(supressCeiling, "supressCeiling");

		fpTileSize = tileSetDef.GetFloat("tileSize");

		Register::Table_c::MatrixDataHandle_c handle = worldEntityTable.GetMatrix("map");

		Render_c &render = Render_c::GetInstance();					

		render.SetAmbientColor(Register::GetColour(worldEntityTable, "ambientColor"));

		Physics::CollisionTag_c staticCollisionTag = GamePhysicsSettings_c::CreateStaticWorldCollisionTag();

		for(int i = 0, numRows = handle.GetNumRows(); i < numRows; ++i)
		{
			for(int j = 0, numCols = handle.GetNumColumns(); j < numCols; ++j)
			{
				switch(handle(i, j))
				{
					case '#':
						break;

					case '.':
						this->CreateFloorMesh(i, j, floorMeshName, tileScale, floorMaterial, staticCollisionTag);

						if(!supressCeiling)
							this->CreateCeilingMesh(i, j, floorMeshName, tileScale, ceilingMaterial, staticCollisionTag);

						if(HasNorthWall(handle, i, j))
						{
							this->CreateNorthWallMesh(i, j, wallMeshName, tileScale, wallMaterial, staticCollisionTag);
						}

						if(HasSouthWall(handle, i, j))						
						{
							this->CreateSouthWallMesh(i, j, wallMeshName, tileScale, wallMaterial, staticCollisionTag);
						}

						if(HasWestWall(handle, i, j))
						{
							this->CreateWestWallMesh(i, j, wallMeshName, tileScale, wallMaterial, staticCollisionTag);
						}

						if(HasEastWall(handle, i, j))						
						{
							this->CreateEastWallMesh(i, j, wallMeshName, tileScale, wallMaterial, staticCollisionTag);
						}

						break;
				}
			}
		}

		const String_t *columnMeshName = tileSetDef.TryGetString("columnMesh");
		if(columnMeshName)
		{
			Ogre::Vector3 scale = Register::TryGetVector3(tileSetDef, "columMeshScale", Ogre::Vector3(1, 1, 1));

			for(int i = 0, numRows = handle.GetNumRows(); i < numRows; ++i)
			{
				for(int j = 0, numCols = handle.GetNumColumns(); j < numCols; ++j)
				{
					if(handle(i, j) != '.')
					{
						continue;
					}

					//North wall
					if(HasNorthWall(handle, i, j))
					{
						//Try the ##
						//        #X

						if((j > 0) && (handle(i-1, j-1) == '#'))
						{
							TileTransform_c::Direction_e dir = IsWestFloor(handle, i, j) ? TileTransform_c::DIR_SOUTH : TileTransform_c::DIR_SOUTH_EAST;
							
							TileTransform_c tileTransform(i, j, dir, TileTransform_c::HGT_MIDDLE, TileTransform_c::POS_NORTH_WEST);

							this->SpawnMesh(tileTransform, *columnMeshName, scale, NULL, staticCollisionTag);													
						}
					}

					//
					//east wall
					if(HasEastWall(handle, i, j))
					{
						//Try the ##
						//        X#
						if((i > 0) && handle(i-1,j+1) == '#')
						{
							TileTransform_c::Direction_e dir = IsNorthFloor(handle, i, j) ? TileTransform_c::DIR_WEST : TileTransform_c::DIR_SOUTH_WEST;
							
							TileTransform_c tileTransform(i, j, dir, TileTransform_c::HGT_MIDDLE, TileTransform_c::POS_NORTH_EAST);

							this->SpawnMesh(tileTransform, *columnMeshName, scale, NULL, staticCollisionTag);
						}
					}

					//
					//south wall
					if(HasSouthWall(handle, i, j))
					{
						//Try the X#
						//        ##
						if((j < handle.GetNumColumns()-1) && (handle(i+1, j+1) == '#'))							
						{
							TileTransform_c::Direction_e dir = IsEastFloor(handle, i, j) ? TileTransform_c::DIR_NORTH : TileTransform_c::DIR_NORTH_WEST;
							
							TileTransform_c tileTransform(i, j, dir, TileTransform_c::HGT_MIDDLE, TileTransform_c::POS_SOUTH_EAST);

							this->SpawnMesh(tileTransform, *columnMeshName, scale, NULL, staticCollisionTag);
						}
					}
					

					//
					//west wall
					if(HasWestWall(handle, i, j))
					{
						//Try the #X
						//        ##
						if((i < handle.GetNumRows()-1) && handle(i+1,j-1) == '#')
						{
							TileTransform_c::Direction_e dir = IsSouthFloor(handle, i, j) ? TileTransform_c::DIR_EAST : TileTransform_c::DIR_NORTH_EAST;
							
							TileTransform_c tileTransform(i, j, dir, TileTransform_c::HGT_MIDDLE, TileTransform_c::POS_SOUTH_WEST);

							this->SpawnMesh(tileTransform, *columnMeshName, scale, NULL, staticCollisionTag);
						}
					}
				}
			}
		}

		const auto &hive = loader.GetStaticEntitiesHive();

		for(Node_c::const_iterator it = hive.begin(), end = hive.end(); it != end; ++it)
		{
			auto *dict = static_cast<Register::Table_c *>(it->second);

			try
			{				
				const String_t &type = dict->GetString("type");

				if(type.compare("Light") == 0)
				{
					const String_t &lightType = dict->GetString("lightType");
					if(lightType.compare("Point") == 0)
					{
						TempStaticObject_s obj;

						this->CreateStaticObjectNode(obj, *dict, Ogre::Vector3(1, 1, 1));										

						obj.pclLight = render.CreateLight();
						obj.pclLight->setType(Ogre::Light::LT_POINT);
						obj.pclLight->setCastShadows(true);

						float attenuation[4];
						if(dict->TryGet4Float(attenuation, "attenuation"))
						{
							dict->Get4Float(attenuation, "attenuation");
							obj.pclLight->setAttenuation(attenuation[0], attenuation[1], attenuation[2], attenuation[3]);
						}
						
						float radius;
						if(dict->TryGetFloat(radius, "radius"))
						{
							//based on: http://imdoingitwrong.wordpress.com/2011/01/31/light-attenuation/
							obj.pclLight->setAttenuation(radius, 1, 2 / radius, 1 / (radius * radius));
						}

						obj.pclLight->setDiffuseColour(Register::GetColour(*dict, "diffuse"));
						obj.pclLight->setSpecularColour(Register::GetColour(*dict, "specular"));

						obj.pclSceneNode->attachObject(obj.pclLight);

						this->CommitTempObject(obj);
					}
					else
					{
						Kernel_c::GetInstance().LogStream() << "[TileGameWorld_c::Load] Unknown light object type: " << lightType;
					}
				}
				else if(type.compare("Model") == 0)
				{
					this->SpawnMesh(this->CreateTileTransform(*dict), dict->GetString("meshfile"), Register::GetVector3(*dict, "scale"), dict->TryGetString("meshMaterial"), staticCollisionTag);					
				}
				else
				{
					Kernel_c::GetInstance().LogStream() << "[TileGameWorld_c::Load] Unknown static object type: " << type;
				}
			}
			catch(Exception_c &ex)
			{
				Kernel_c::GetInstance().LogStream() << "[TileGameWorld_c::Load] Exception loading static object: " << ex.what();
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
		{"northWest", TileTransform_c::DIR_NORTH_WEST},
		{"northEast", TileTransform_c::DIR_NORTH_EAST},
		{"up", TileTransform_c::DIR_NORTH},		
		{"east", TileTransform_c::DIR_EAST},
		{"right", TileTransform_c::DIR_EAST},
		{"south", TileTransform_c::DIR_SOUTH},
		{"southWest", TileTransform_c::DIR_SOUTH_WEST},
		{"southEast", TileTransform_c::DIR_SOUTH_EAST},
		{"down", TileTransform_c::DIR_SOUTH},
		{"west", TileTransform_c::DIR_WEST},
		{"left", TileTransform_c::DIR_WEST},
		{NULL, TileTransform_c::DIR_NORTH}
	};

	struct HeightType_s
	{
		const char						*pstrzName;
		TileTransform_c::Height_e		eValue;
	};

	static HeightType_s stHeightTypes_gl[] =
	{
		{"floor", TileTransform_c::HGT_FLOOR},
		{"middle", TileTransform_c::HGT_MIDDLE},
		{"ceiling", TileTransform_c::HGT_CEILING},
		{"aboveFloor", TileTransform_c::HGT_ABOVE_FLOOR},
		{"belowCeiling", TileTransform_c::HGT_BELOW_CEILING},
		{NULL, TileTransform_c::HGT_FLOOR}
	};

	struct PositionType_s
	{
		const char *pstrzName;
		TileTransform_c::Position_e	eValue;
	};

	static PositionType_s stPositionTypes_gl[] = 
	{
		{"center", TileTransform_c::POS_CENTER},
		{"northWest", TileTransform_c::POS_NORTH_WEST},
		{"northCentered", TileTransform_c::POS_NORTH_CENTERED},
		{"northEast", TileTransform_c::POS_NORTH_EAST},
		{"eastCentered", TileTransform_c::POS_EAST_CENTERED},
		{"sourthCentered", TileTransform_c::POS_SOUTH_CENTERED},
		{"sourthWest", TileTransform_c::POS_SOUTH_WEST},
		{"westCentered", TileTransform_c::POS_WEST_CENTERED},
		{NULL, TileTransform_c::POS_CENTER}
	};

	Ogre::Vector3 TileGameWorld_c::CalculatePosition(int row, int col) const
	{
		return Ogre::Vector3((fpTileSize * col) + fpTileSize / 2, 0, (fpTileSize * row) + fpTileSize / 2);
	}

	void TileGameWorld_c::TileTransform2Transform(Transform_c &out, const TileTransform_c &tileTransform) const
	{				
		out.SetOrigin(this->CalculatePosition(tileTransform.GetRow(), tileTransform.GetCol()));
		out.SetRotation(Ogre::Quaternion(Ogre::Degree(static_cast<Ogre::Real>(tileTransform.GetDirection())), Ogre::Vector3::UNIT_Y));

		Ogre::Vector3 translation(0, 0, 0);

		switch(tileTransform.GetHeight())
		{
			case TileTransform_c::HGT_FLOOR:
				break;

			case TileTransform_c::HGT_MIDDLE:
				translation.y = fpTileSize / 2;
				break;

			case TileTransform_c::HGT_CEILING:
				translation.y = fpTileSize;
				break;

			case TileTransform_c::HGT_ABOVE_FLOOR:
				translation.y = fpTileSize * 0.1f;				
				break;

			case TileTransform_c::HGT_BELOW_CEILING:
				translation.y = fpTileSize - (fpTileSize * 0.1f);				
				break;
		}		

		switch(tileTransform.GetPosition())
		{
			case TileTransform_c::POS_NORTH_WEST:
				translation.x = -fpTileSize / 2.0f;
				translation.z = -fpTileSize / 2.0f;
				break;

			case TileTransform_c::POS_NORTH_CENTERED:				
				translation.z = -fpTileSize / 2.0f;
				break;

			case TileTransform_c::POS_NORTH_EAST:
				translation.x = fpTileSize / 2.0f;
				translation.z = -fpTileSize / 2.0f;
				break;

			case TileTransform_c::POS_EAST_CENTERED:
				translation.x = fpTileSize / 2.0f;				
				break;

			case TileTransform_c::POS_SOUTH_EAST:
				translation.x = fpTileSize / 2.0f;				
				translation.z = fpTileSize / 2.0f;
				break;

			case TileTransform_c::POS_SOUTH_CENTERED:							
				translation.z = fpTileSize / 2.0f;
				break;

			case TileTransform_c::POS_SOUTH_WEST:
				translation.x = -fpTileSize / 2.0f;				
				translation.z = fpTileSize / 2.0f;
				break;

			case TileTransform_c::POS_WEST_CENTERED:
				translation.x = -fpTileSize / 2.0f;								
				break;
		}

		out.Translate(translation);
	}
	
	void TileGameWorld_c::LoadTileTransform(Transform_c &out, const Register::Table_c &entity) const
	{
		this->TileTransform2Transform(out, this->CreateTileTransform(entity));
	}

	TileTransform_c  TileGameWorld_c::CreateTileTransform(const Register::Table_c &entity) const
	{
		static Enum_c<TileTransform_c::Direction_e, DirectionType_s> clDirectionType_gl(stDirectionTypes_gl);
		static Enum_c<TileTransform_c::Height_e, HeightType_s> clHeightType_gl(stHeightTypes_gl);
		static Enum_c<TileTransform_c::Position_e, PositionType_s> clPositionType_gl(stPositionTypes_gl);

		const int row = entity.GetInt(PH_ENTITY_KEY_TILE_ROW);		
		const int col = entity.GetInt(PH_ENTITY_KEY_TILE_COL);

		const String_t &direction = entity.GetString("faces");

		TileTransform_c::Direction_e dir;
		if(!clDirectionType_gl.TryGetValue(dir, direction))
		{
			std::stringstream stream;
			stream << "Invalid value for faces parameters: " << direction;

			PH_RAISE(INVALID_PARAMETER_EXCEPTION, "TileGameWorld_c::LoadTileTransform", stream.str());
		}

		TileTransform_c::Height_e height = TileTransform_c::HGT_FLOOR;
				
		const String_t *heightStr = entity.TryGetString("tileHeight");
		if(heightStr != NULL)
		{			
			if(!clHeightType_gl.TryGetValue(height, *heightStr))
			{
				std::stringstream stream;
				stream << "Invalid value for tileHeight parameters: " << (*heightStr);

				PH_RAISE(INVALID_PARAMETER_EXCEPTION, "TileGameWorld_c::LoadTileTransform", stream.str());
			}						
		}

		TileTransform_c::Position_e position = TileTransform_c::POS_CENTER;
				
		const String_t *positionStr = entity.TryGetString("tilePosition");
		if(positionStr != NULL)
		{			
			if(!clPositionType_gl.TryGetValue(position, *positionStr))
			{
				std::stringstream stream;
				stream << "Invalid value for tilePosition parameters: " << (*positionStr);

				PH_RAISE(INVALID_PARAMETER_EXCEPTION, "TileGameWorld_c::LoadTileTransform", stream.str());
			}						
		}

		return TileTransform_c(row, col, dir, height, position);		
	}
}
