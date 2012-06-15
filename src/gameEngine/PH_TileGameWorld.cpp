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
		BOOST_FOREACH(StaticObject_s &obj, vecObjects)
		{
			obj.Clear();
		}
	}

	void TileGameWorld_c::CommitTempObject(TempStaticObject_s &obj)
	{
		StaticObject_s object;
		obj.Commit(object);

		vecObjects.push_back(object);
	}

	void TileGameWorld_c::CreateStaticObjectNode(TempStaticObject_s &obj, const Dictionary_c &dict)
	{
		obj.pclSceneNode = Render_c::GetInstance()->CreateSceneNode();

		Transform_c transform;
		this->LoadTileTransform(transform, dict);

		obj.pclSceneNode->setPosition(transform.GetOrigin());
		obj.pclSceneNode->setOrientation(transform.GetRotation());
	}

	void TileGameWorld_c::CreateStaticObjectMesh(TempStaticObject_s &obj, const String_c &meshName)
	{
		obj.pclEntity = Render_c::GetInstance()->CreateEntity(meshName);
		obj.pclEntity->setCastShadows(true);

		obj.pclSceneNode->attachObject(obj.pclEntity);
	}

	void TileGameWorld_c::CreateMesh(int row, int col, const String_c &meshName, Float_t tileScale, const Transform_c &transform)
	{
		TempStaticObject_s obj;

		RenderPtr_t render = Render_c::GetInstance();
		obj.pclSceneNode = render->CreateSceneNode();

		this->CreateStaticObjectMesh(obj, meshName);

		obj.pclSceneNode->setScale(tileScale, tileScale, tileScale);
		obj.pclSceneNode->setPosition(this->CalculatePosition(row, col));

		obj.pclSceneNode->translate(transform.GetOrigin());
		obj.pclSceneNode->rotate(transform.GetRotation());

		this->CommitTempObject(obj);		
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
				
		//render->SetShadowMode(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

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

		const DictionaryHive_c &hive = loader.GetStaticEntitiesHive();

		for(Node_c::const_iterator it = hive.begin(), end = hive.end(); it != end; ++it)
		{
			DictionaryPtr_t dict = boost::static_pointer_cast<Dictionary_c>(it->second);

			try
			{				
				const String_c &type = dict->GetString("type");

				if(type.compare("Light") == 0)
				{
					const String_c &lightType = dict->GetString("lightType");
					if(lightType.compare("Point") == 0)
					{
						TempStaticObject_s obj;

						this->CreateStaticObjectNode(obj, *dict.get());										

						obj.pclLight = render->CreateLight();
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

						obj.pclLight->setDiffuseColour(DictionaryGetColour(*dict, "diffuse"));
						obj.pclLight->setSpecularColour(DictionaryGetColour(*dict, "specular"));

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
					TempStaticObject_s obj;
						
					this->CreateStaticObjectNode(obj, *dict.get());
					
					obj.pclSceneNode->setScale(DictionaryGetVector3(*dict, "scale"));

					const String_c &meshName = dict->GetString("meshfile");
					this->CreateStaticObjectMesh(obj, meshName);

					this->CommitTempObject(obj);
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
		{"up", TileTransform_c::DIR_NORTH},		
		{"east", TileTransform_c::DIR_EAST},
		{"right", TileTransform_c::DIR_EAST},
		{"south", TileTransform_c::DIR_SOUTH},
		{"down", TileTransform_c::DIR_SOUTH},
		{"west", TileTransform_c::DIR_WEST},
		{"left", TileTransform_c::DIR_WEST}
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
	};

	Ogre::Vector3 TileGameWorld_c::CalculatePosition(int row, int col) const
	{
		return Ogre::Vector3((fpTileSize * col) + fpTileSize / 2, 0, (fpTileSize * row) + fpTileSize / 2);
	}
	
	void TileGameWorld_c::LoadTileTransform(Transform_c &out, const Dictionary_c &entity) const
	{
		static Enum_c<TileTransform_c::Direction_e, DirectionType_s> clDirectionType_gl(stDirectionTypes_gl);
		static Enum_c<TileTransform_c::Height_e, HeightType_s> clHeightType_gl(stHeightTypes_gl);

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

		const String_c *heightStr = entity.TryGetString("tileHeight");
		if(heightStr != NULL)
		{
			TileTransform_c::Height_e height;

			if(!clHeightType_gl.TryGetValue(height, *heightStr))
			{
				std::stringstream stream;
				stream << "Invalid value for tileHeight parameters: " << (*heightStr);

				PH_RAISE(INVALID_PARAMETER_EXCEPTION, "TileGameWorld_c::LoadTileTransform", stream.str());
			}

			switch(height)
			{
				case TileTransform_c::HGT_FLOOR:
					break;

				case TileTransform_c::HGT_MIDDLE:
					out.Translate(Ogre::Vector3(0, fpTileSize / 2, 0));
					break;

				case TileTransform_c::HGT_CEILING:
					out.Translate(Ogre::Vector3(0, fpTileSize, 0));
					break;

				case TileTransform_c::HGT_ABOVE_FLOOR:
					out.Translate(Ogre::Vector3(0, fpTileSize * 0.1f, 0));
					break;

				case TileTransform_c::HGT_BELOW_CEILING:
					out.Translate(Ogre::Vector3(0, fpTileSize - (fpTileSize * 0.1f), 0));
					break;
			}
		}
	}
}
