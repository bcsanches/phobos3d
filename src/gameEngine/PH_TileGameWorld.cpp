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

#include <OgreEntity.h>

#include <Phobos/Enum.h>
#include <Phobos/Log.h>

#include <Phobos/OgreEngine/Render.h>
#include <Phobos/OgreEngine/Math/Transform.h>

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
	static inline bool CheckNorthTileEqual(const Register::Table::MatrixDataHandle &handle, int row, int col, char tile)
	{
		return (row > 0) && handle(row-1, col) == tile;
	}

	static inline bool CheckWestTileEqual(const Register::Table::MatrixDataHandle &handle, int row, int col, char tile)
	{
		return (col > 0) && handle(row, col-1) == tile;
	}

	static inline bool CheckSouthTileEqual(const Register::Table::MatrixDataHandle &handle, int row, int col, char tile)
	{
		return (row < handle.GetNumRows() - 1) && handle(row+1, col) == tile;
	}		

	static inline bool CheckEastTileEqual(const Register::Table::MatrixDataHandle &handle, int row, int col, char tile)
	{
		return (col < handle.GetNumColumns()-1) && handle(row, col+1) == tile;
	}

	static inline bool HasNorthWall(const Register::Table::MatrixDataHandle &handle, int row, int col)
	{
		return CheckNorthTileEqual(handle, row, col, '#');
	}

	static inline bool HasWestWall(const Register::Table::MatrixDataHandle &handle, int row, int col)
	{
		return CheckWestTileEqual(handle, row, col, '#');
	}

	static inline bool HasSouthWall(const Register::Table::MatrixDataHandle &handle, int row, int col)
	{
		return CheckSouthTileEqual(handle, row, col, '#');
	}	

	static inline bool HasEastWall(const Register::Table::MatrixDataHandle &handle, int row, int col)
	{
		return CheckEastTileEqual(handle, row, col, '#');
	}

	static inline bool IsNorthFloor(const Register::Table::MatrixDataHandle &handle, int row, int col)
	{
		return CheckNorthTileEqual(handle, row, col, '.');
	}

	static inline bool IsWestFloor(const Register::Table::MatrixDataHandle &handle, int row, int col)
	{
		return CheckWestTileEqual(handle, row, col, '.');
	}

	static inline bool IsSouthFloor(const Register::Table::MatrixDataHandle &handle, int row, int col)
	{
		return CheckSouthTileEqual(handle, row, col, '.');
	}	

	static inline bool IsEastFloor(const Register::Table::MatrixDataHandle &handle, int row, int col)
	{
		return CheckEastTileEqual(handle, row, col, '.');
	}

	TileGameWorld::TileGameWorld():
		BaseOgreGameWorld()
	{
		//empty
	}

	TileGameWorld::~TileGameWorld()
	{		
		for(StaticObject_s &obj : m_vecObjects)
		{
			obj.Clear();
		}
	}

	void TileGameWorld::CreateStaticObjectRigidBody(StaticObject_s &staticObj, const Ogre::Vector3 &scale, const Physics::CollisionTag &collisionTag)
	{
		Engine::Math::Transform transform(staticObj.m_pclSceneNode->_getDerivedPosition(), staticObj.m_pclSceneNode->_getDerivedOrientation());

		BaseOgreGameWorld::CreateStaticObjectRigidBody(staticObj, transform, scale, collisionTag);		
	}

	BaseOgreGameWorld::StaticObject_s &TileGameWorld::CommitTempObject(TempStaticObject_s &obj)
	{
		StaticObject_s object;
		obj.Commit(object);

		m_vecObjects.push_back(object);		

		return m_vecObjects.back();
	}

	void TileGameWorld::CreateStaticObjectNode(TempStaticObject_s &obj, const TileTransform &tileTransform, const Ogre::Vector3 &scale)
	{
		obj.m_pclSceneNode = OgreEngine::Render::GetInstance().CreateSceneNode();

		Engine::Math::Transform transform;
		this->TileTransform2Transform(transform, tileTransform);

		obj.m_pclSceneNode->setPosition(transform.GetOrigin());
		obj.m_pclSceneNode->setOrientation(transform.GetRotation());
		obj.m_pclSceneNode->setScale(scale);
	}

	void TileGameWorld::CreateStaticObjectNode(TempStaticObject_s &obj, const Register::Table &table, const Ogre::Vector3 &scale)
	{		
		this->CreateStaticObjectNode(obj, this->CreateTileTransform(table), scale);		
	}

	void TileGameWorld::CreateStaticObjectMesh(TempStaticObject_s &obj, const String_t &meshName, const String_t *optionalMaterial) const
	{
		obj.m_pclEntity = OgreEngine::Render::GetInstance().CreateEntity(meshName);
		obj.m_pclEntity->setCastShadows(true);		

		if(optionalMaterial)
			obj.m_pclEntity->setMaterialName(*optionalMaterial);

		obj.m_pclSceneNode->attachObject(obj.m_pclEntity);
	}

	void TileGameWorld::SpawnMesh(int row, int col, const String_t &meshName, Float_t tileScale, const Engine::Math::Transform &transform, const String_t *optionalMaterial, const Physics::CollisionTag &collisionTag)
	{
		TempStaticObject_s obj;

		auto &render = OgreEngine::Render::GetInstance();
		obj.m_pclSceneNode = render.CreateSceneNode();

		this->CreateStaticObjectMesh(obj, meshName, optionalMaterial);

		obj.m_pclSceneNode->setScale(tileScale, tileScale, tileScale);
		obj.m_pclSceneNode->setPosition(this->CalculatePosition(row, col));

		obj.m_pclSceneNode->translate(transform.GetOrigin());
		obj.m_pclSceneNode->rotate(transform.GetRotation());		

		this->CreateStaticObjectRigidBody(this->CommitTempObject(obj), Ogre::Vector3(tileScale, tileScale, tileScale), collisionTag);
	}
	
	void TileGameWorld::SpawnMesh(const TileTransform tileTransform, const String_t &meshName, const Ogre::Vector3 &scale, const String_t *optionalMaterial, const Physics::CollisionTag &collisionTag)
	{
		TempStaticObject_s obj;							

		this->CreateStaticObjectNode(obj, tileTransform, scale);
		this->CreateStaticObjectMesh(obj, meshName, optionalMaterial);

		this->CreateStaticObjectRigidBody(this->CommitTempObject(obj), scale, collisionTag);
	}

	void TileGameWorld::CreateCeilingMesh(int row, int col, const String_t &meshName, Float_t tileScale, const String_t *optionalMaterial, const Physics::CollisionTag &collisionTag)
	{		
		this->SpawnMesh(row, col, meshName, tileScale, Engine::Math::Transform(Ogre::Vector3(0, m_fpTileSize, 0), Ogre::Quaternion(Ogre::Degree(180), Ogre::Vector3::UNIT_X)), optionalMaterial, collisionTag);
	}

	void TileGameWorld::CreateFloorMesh(int row, int col, const String_t &meshName, Float_t tileScale, const String_t *optionalMaterial, const Physics::CollisionTag &collisionTag)
	{
		this->SpawnMesh(row, col, meshName, tileScale, Engine::Math::Transform(), optionalMaterial, collisionTag);						
	}

	void TileGameWorld::CreateNorthWallMesh(int row, int col, const String_t &meshName, Float_t tileScale, const String_t *optionalMaterial, const Physics::CollisionTag &collisionTag)
	{
		this->SpawnMesh(row, col, meshName, tileScale, Engine::Math::Transform(Ogre::Vector3(0, m_fpTileSize/2, -m_fpTileSize/2), Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3::UNIT_X)), optionalMaterial, collisionTag);
	}
	
	void TileGameWorld::CreateSouthWallMesh(int row, int col, const String_t &meshName, Float_t tileScale, const String_t *optionalMaterial, const Physics::CollisionTag &collisionTag)
	{
		this->SpawnMesh(
			row, 
			col, 
			meshName, 
			tileScale, 
			Engine::Math::Transform(
				Ogre::Vector3(0, m_fpTileSize/2, m_fpTileSize/2), Ogre::Quaternion(Ogre::Degree(-90), Ogre::Vector3::UNIT_X) * Ogre::Quaternion(Ogre::Degree(180), Ogre::Vector3::UNIT_Y)
			), 
			optionalMaterial,
			collisionTag
		);
	}
	
	void TileGameWorld::CreateWestWallMesh(int row, int col, const String_t &meshName, Float_t tileScale, const String_t *optionalMaterial, const Physics::CollisionTag &collisionTag)
	{
		this->SpawnMesh(
			row, 
			col, 
			meshName, 
			tileScale, 
			Engine::Math::Transform(
				Ogre::Vector3(-m_fpTileSize/2, m_fpTileSize/2, 0), 
				Ogre::Quaternion(Ogre::Degree(-90), Ogre::Vector3::UNIT_Z) * Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3::UNIT_Y)
			), 
			optionalMaterial,
			collisionTag
		);
	}

	void TileGameWorld::CreateEastWallMesh(int row, int col, const String_t &meshName, Float_t tileScale, const String_t *optionalMaterial, const Physics::CollisionTag &collisionTag)
	{
		this->SpawnMesh(
			row, 
			col, 
			meshName, 
			tileScale, 
			Engine::Math::Transform(
				Ogre::Vector3(m_fpTileSize/2, m_fpTileSize/2, 0), 
				Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3::UNIT_Z) * Ogre::Quaternion(Ogre::Degree(-90), Ogre::Vector3::UNIT_Y)
			),
			optionalMaterial,
			collisionTag
		);
	}

	void TileGameWorld::Load(const MapLoader &loader, const Register::Table &worldEntityTable)
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

		m_fpTileSize = tileSetDef.GetFloat("tileSize");

		Register::Table::MatrixDataHandle handle = worldEntityTable.GetMatrix("map");

		auto &render = OgreEngine::Render::GetInstance();					

		render.SetAmbientColor(Register::GetColour(worldEntityTable, "ambientColor"));

		Physics::CollisionTag staticCollisionTag = GamePhysicsSettings::CreateStaticWorldCollisionTag();

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
							TileTransform::Direction_e dir = IsWestFloor(handle, i, j) ? TileTransform::DIR_SOUTH : TileTransform::DIR_SOUTH_EAST;
							
							TileTransform tileTransform(i, j, dir, TileTransform::HGT_MIDDLE, TileTransform::POS_NORTH_WEST);

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
							TileTransform::Direction_e dir = IsNorthFloor(handle, i, j) ? TileTransform::DIR_WEST : TileTransform::DIR_SOUTH_WEST;
							
							TileTransform tileTransform(i, j, dir, TileTransform::HGT_MIDDLE, TileTransform::POS_NORTH_EAST);

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
							TileTransform::Direction_e dir = IsEastFloor(handle, i, j) ? TileTransform::DIR_NORTH : TileTransform::DIR_NORTH_WEST;
							
							TileTransform tileTransform(i, j, dir, TileTransform::HGT_MIDDLE, TileTransform::POS_SOUTH_EAST);

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
							TileTransform::Direction_e dir = IsSouthFloor(handle, i, j) ? TileTransform::DIR_EAST : TileTransform::DIR_NORTH_EAST;
							
							TileTransform tileTransform(i, j, dir, TileTransform::HGT_MIDDLE, TileTransform::POS_SOUTH_WEST);

							this->SpawnMesh(tileTransform, *columnMeshName, scale, NULL, staticCollisionTag);
						}
					}
				}
			}
		}

		const auto &hive = loader.GetStaticEntitiesHive();

		for(Node::const_iterator it = hive.begin(), end = hive.end(); it != end; ++it)
		{
			auto *dict = static_cast<Register::Table *>(it->second);

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

						obj.m_pclLight = render.CreateLight();
						obj.m_pclLight->setType(Ogre::Light::LT_POINT);
						obj.m_pclLight->setCastShadows(true);

						float attenuation[4];
						if(dict->TryGet4Float(attenuation, "attenuation"))
						{
							dict->Get4Float(attenuation, "attenuation");
							obj.m_pclLight->setAttenuation(attenuation[0], attenuation[1], attenuation[2], attenuation[3]);
						}
						
						float radius;
						if(dict->TryGetFloat(radius, "radius"))
						{
							//based on: http://imdoingitwrong.wordpress.com/2011/01/31/light-attenuation/
							obj.m_pclLight->setAttenuation(radius, 1, 2 / radius, 1 / (radius * radius));
						}

						obj.m_pclLight->setDiffuseColour(Register::GetColour(*dict, "diffuse"));
						obj.m_pclLight->setSpecularColour(Register::GetColour(*dict, "specular"));

						obj.m_pclSceneNode->attachObject(obj.m_pclLight);

						this->CommitTempObject(obj);
					}
					else
					{
						LogMakeStream() << "[TileGameWorld::Load] Unknown light object type: " << lightType;
					}
				}
				else if(type.compare("Model") == 0)
				{
					this->SpawnMesh(this->CreateTileTransform(*dict), dict->GetString("meshfile"), Register::GetVector3(*dict, "scale"), dict->TryGetString("meshMaterial"), staticCollisionTag);					
				}
				else
				{
					LogMakeStream() << "[TileGameWorld::Load] Unknown static object type: " << type;
				}
			}
			catch(Exception &ex)
			{
				LogMakeStream() << "[TileGameWorld::Load] Exception loading static object: " << ex.what();
			}
		}
	}

	struct DirectionType_s
	{
		const char						*pstrzName;
		TileTransform::Direction_e	eValue;
	};

	static DirectionType_s stDirectionTypes_gl[] =
	{
		{"north", TileTransform::DIR_NORTH},
		{"northWest", TileTransform::DIR_NORTH_WEST},
		{"northEast", TileTransform::DIR_NORTH_EAST},
		{"up", TileTransform::DIR_NORTH},		
		{"east", TileTransform::DIR_EAST},
		{"right", TileTransform::DIR_EAST},
		{"south", TileTransform::DIR_SOUTH},
		{"southWest", TileTransform::DIR_SOUTH_WEST},
		{"southEast", TileTransform::DIR_SOUTH_EAST},
		{"down", TileTransform::DIR_SOUTH},
		{"west", TileTransform::DIR_WEST},
		{"left", TileTransform::DIR_WEST},
		{NULL, TileTransform::DIR_NORTH}
	};

	struct HeightType_s
	{
		const char						*pstrzName;
		TileTransform::Height_e		eValue;
	};

	static HeightType_s stHeightTypes_gl[] =
	{
		{"floor", TileTransform::HGT_FLOOR},
		{"middle", TileTransform::HGT_MIDDLE},
		{"ceiling", TileTransform::HGT_CEILING},
		{"aboveFloor", TileTransform::HGT_ABOVE_FLOOR},
		{"belowCeiling", TileTransform::HGT_BELOW_CEILING},
		{NULL, TileTransform::HGT_FLOOR}
	};

	struct PositionType_s
	{
		const char *pstrzName;
		TileTransform::Position_e	eValue;
	};

	static PositionType_s stPositionTypes_gl[] = 
	{
		{"center", TileTransform::POS_CENTER},
		{"northWest", TileTransform::POS_NORTH_WEST},
		{"northCentered", TileTransform::POS_NORTH_CENTERED},
		{"northEast", TileTransform::POS_NORTH_EAST},
		{"eastCentered", TileTransform::POS_EAST_CENTERED},
		{"sourthCentered", TileTransform::POS_SOUTH_CENTERED},
		{"sourthWest", TileTransform::POS_SOUTH_WEST},
		{"westCentered", TileTransform::POS_WEST_CENTERED},
		{NULL, TileTransform::POS_CENTER}
	};

	Ogre::Vector3 TileGameWorld::CalculatePosition(int row, int col) const
	{
		return Ogre::Vector3((m_fpTileSize * col) + m_fpTileSize / 2, 0, (m_fpTileSize * row) + m_fpTileSize / 2);
	}

	void TileGameWorld::TileTransform2Transform(Engine::Math::Transform &out, const TileTransform &tileTransform) const
	{				
		out.SetOrigin(this->CalculatePosition(tileTransform.GetRow(), tileTransform.GetCol()));
		out.SetRotation(Ogre::Quaternion(Ogre::Degree(static_cast<Ogre::Real>(tileTransform.GetDirection())), Ogre::Vector3::UNIT_Y));

		Ogre::Vector3 translation(0, 0, 0);

		switch(tileTransform.GetHeight())
		{
			case TileTransform::HGT_FLOOR:
				break;

			case TileTransform::HGT_MIDDLE:
				translation.y = m_fpTileSize / 2;
				break;

			case TileTransform::HGT_CEILING:
				translation.y = m_fpTileSize;
				break;

			case TileTransform::HGT_ABOVE_FLOOR:
				translation.y = m_fpTileSize * 0.1f;				
				break;

			case TileTransform::HGT_BELOW_CEILING:
				translation.y = m_fpTileSize - (m_fpTileSize * 0.1f);				
				break;
		}		

		switch(tileTransform.GetPosition())
		{
			case TileTransform::POS_NORTH_WEST:
				translation.x = -m_fpTileSize / 2.0f;
				translation.z = -m_fpTileSize / 2.0f;
				break;

			case TileTransform::POS_NORTH_CENTERED:				
				translation.z = -m_fpTileSize / 2.0f;
				break;

			case TileTransform::POS_NORTH_EAST:
				translation.x = m_fpTileSize / 2.0f;
				translation.z = -m_fpTileSize / 2.0f;
				break;

			case TileTransform::POS_EAST_CENTERED:
				translation.x = m_fpTileSize / 2.0f;				
				break;

			case TileTransform::POS_SOUTH_EAST:
				translation.x = m_fpTileSize / 2.0f;				
				translation.z = m_fpTileSize / 2.0f;
				break;

			case TileTransform::POS_SOUTH_CENTERED:							
				translation.z = m_fpTileSize / 2.0f;
				break;

			case TileTransform::POS_SOUTH_WEST:
				translation.x = -m_fpTileSize / 2.0f;				
				translation.z = m_fpTileSize / 2.0f;
				break;

			case TileTransform::POS_WEST_CENTERED:
				translation.x = -m_fpTileSize / 2.0f;								
				break;
		}

		out.Translate(translation);
	}
	
	void TileGameWorld::LoadTileTransform(Engine::Math::Transform &out, const Register::Table &entity) const
	{
		this->TileTransform2Transform(out, this->CreateTileTransform(entity));
	}

	TileTransform  TileGameWorld::CreateTileTransform(const Register::Table &entity) const
	{
		static Enum<TileTransform::Direction_e, DirectionType_s> clDirectionType_gl(stDirectionTypes_gl);
		static Enum<TileTransform::Height_e, HeightType_s> clHeightType_gl(stHeightTypes_gl);
		static Enum<TileTransform::Position_e, PositionType_s> clPositionType_gl(stPositionTypes_gl);

		const int row = entity.GetInt(PH_ENTITY_KEY_TILE_ROW);		
		const int col = entity.GetInt(PH_ENTITY_KEY_TILE_COL);

		const String_t &direction = entity.GetString("faces");

		TileTransform::Direction_e dir;
		if(!clDirectionType_gl.TryGetValue(dir, direction))
		{
			std::stringstream stream;
			stream << "Invalid value for faces parameters: " << direction;

			PH_RAISE(INVALID_PARAMETER_EXCEPTION, "TileGameWorld::LoadTileTransform", stream.str());
		}

		TileTransform::Height_e height = TileTransform::HGT_FLOOR;
				
		const String_t *heightStr = entity.TryGetString("tileHeight");
		if(heightStr != NULL)
		{			
			if(!clHeightType_gl.TryGetValue(height, *heightStr))
			{
				std::stringstream stream;
				stream << "Invalid value for tileHeight parameters: " << (*heightStr);

				PH_RAISE(INVALID_PARAMETER_EXCEPTION, "TileGameWorld::LoadTileTransform", stream.str());
			}						
		}

		TileTransform::Position_e position = TileTransform::POS_CENTER;
				
		const String_t *positionStr = entity.TryGetString("tilePosition");
		if(positionStr != NULL)
		{			
			if(!clPositionType_gl.TryGetValue(position, *positionStr))
			{
				std::stringstream stream;
				stream << "Invalid value for tilePosition parameters: " << (*positionStr);

				PH_RAISE(INVALID_PARAMETER_EXCEPTION, "TileGameWorld::LoadTileTransform", stream.str());
			}						
		}

		return TileTransform(row, col, dir, height, position);		
	}
}
