#include "Editor/PH_AssetListRequest.h"

#include "Editor/PH_RequestFactory.h"

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


void Phobos::Editor::AssetListRequest::OnExecute(JsonCreator::StringWriter &response)
{
	Ogre::StringVectorPtr pList = Ogre::ResourceGroupManager::getSingleton().findResourceNames("PH_GameData","*.mesh",false);
	
	{
		auto obj = JsonCreator::MakeObject(response);

		obj.AddStringValue("command", "AssetListResponse");

		{
			auto assetArray = obj.AddArray("assets");

			for(Ogre::StringVector::iterator it = pList->begin(), end = pList->end(); it != end;++it)
			{
				auto assetObj = assetArray.AddObject();

				assetObj.AddStringValue("name", it->c_str());
				assetObj.AddStringValue("category", "static");
			}
		}
	}
}
