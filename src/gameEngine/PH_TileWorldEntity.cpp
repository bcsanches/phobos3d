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

#include "PH_TileWorldEntity.h"

#include <boost/foreach.hpp>

#include <OgreEntity.h>

#include <PH_Render.h>

#include "PH_Dictionary.h"
#include "PH_DictionaryHive.h"
#include "PH_DictionaryManager.h"
#include "PH_EntityFactory.h"

namespace Phobos
{
	PH_FULL_ENTITY_CREATOR("TileWorldEntity", TileWorldEntity_c);

	TileWorldEntity_c::TileWorldEntity_c(const String_c &name):
		BaseOgreWorldEntity_c(name)
	{
		//empty
	}

	TileWorldEntity_c::~TileWorldEntity_c()
	{
		RenderPtr_t render = Render_c::GetInstance();

		BOOST_FOREACH(StaticObject_s &obj, vecObjects)
		{
			render->DestroySceneNode(obj.pclSceneNode);
			render->DestroyEntity(obj.pclEntity);
		}
	}

	BaseOgreWorldEntity_c::StaticObject_s TileWorldEntity_c::CreateFloorMesh(int row, int col, const String_c &meshName, Float_t tileScale, Float_t tileSize)
	{
		TempStaticObject_s temp;

		RenderPtr_t render = Render_c::GetInstance();
		temp.pclSceneNode = render->CreateSceneNode();

		temp.pclEntity = render->CreateEntity(meshName);

		temp.pclSceneNode->attachObject(temp.pclEntity);

		temp.pclSceneNode->setScale(tileScale, tileScale, tileScale);
		temp.pclSceneNode->setPosition((tileSize * col) + tileSize / 2, 0, (tileSize * row) + tileSize / 2);

		StaticObject_s object;
		temp.Commit(object);

		return object;
	}


	void TileWorldEntity_c::LoadMap(const MapLoader_c &loader)
	{		
		const Dictionary_c &entityDef = this->GetDictionary();

		DictionaryPtr_t tileSetDef = DictionaryManager_c::GetInstance()->GetDictionaryHive("TileSet")->GetDictionary(entityDef.GetString("tileSet"));

		const String_c &wallMeshName = tileSetDef->GetString("wall");
		const String_c &floorMeshName = tileSetDef->GetString("floor");
		const String_c &ceilingMeshName = tileSetDef->GetString("ceiling");
		const Float_t tileSize = tileSetDef->GetFloat("tileSize");
		const Float_t tileScale = tileSetDef->GetFloat("tileScale");

		Dictionary_c::MatrixDataHandle_c handle = entityDef.GetMatrix("map");

		RenderPtr_t render = Render_c::GetInstance();

		for(int i = 0, numRows = handle.GetNumRows(); i < numRows; ++i)
		{
			for(int j = 0, numCols = handle.GetNumColumns(); j < numCols; ++j)
			{
				switch(handle(i, j))
				{
					case 'X':
						break;

					case '.':
						vecObjects.push_back(this->CreateFloorMesh(i, j, floorMeshName, tileScale, tileSize));
						break;
				}
			}
		}
		
	}
}
