#include "Phobos/Editor/AssetListRequest.h"

#include "Phobos/Editor/RequestFactory.h"

#include <Phobos/Path.h>

#include <Phobos/Register/Hive.h>
#include <Phobos/Register/Manager.h>
#include <Phobos/Register/Table.h>

#include <Phobos/Game/Level/MapDefs.h>

#include <OgreResourceGroupManager.h>

#include <JsonCreator/Object.h>
#include <JsonCreator/StringWriter.h>

namespace Phobos
{
	namespace Editor
	{
		PH_FULL_REQUEST_CREATOR("AssetList", AssetListRequest);
	}
}

Phobos::Editor::AssetListRequest::AssetListRequest(const rapidjson::Value &value):
	Request(value)
{
	//empty
}


void Phobos::Editor::AssetListRequest::OnExecute(const rapidjson::Value *parameters, JsonCreator::Object<JsonCreator::StringWriter> *response)
{			
	auto assetArray = response->AddArray("result");
		
	Ogre::StringVectorPtr pList = Ogre::ResourceGroupManager::getSingleton().findResourceNames("PH_GameData","*.mesh",false);			

	for(Ogre::StringVector::iterator it = pList->begin(), end = pList->end(); it != end;++it)
	{
		Path path(it->c_str());
		
		path.StripExtension();

		auto assetObj = assetArray.AddObject();

		assetObj.AddStringValue("name", path.GetStr().c_str());
		assetObj.AddStringValue("category", "static");
		assetObj.AddStringValue("type", Game::MapObjectTypeToString(Game::MapObjectTypes::STATIC));
	}

	const auto &hive = Register::GetHive("MapObjectDef");
						
	for(auto it : hive)
	{												
		auto table = static_cast<Register::Table *>(it.second);

		auto assetObj = assetArray.AddObject();

		auto strCategory = table->TryGetString("category");
		auto category = strCategory ? strCategory->c_str() : "entity";

		assetObj.AddStringValue("name", table->GetName().c_str());
		assetObj.AddStringValue("category", category);
		assetObj.AddStringValue("type", Game::MapObjectTypeToString(Game::MapObjectTypes::ENTITY));
	}	
}
