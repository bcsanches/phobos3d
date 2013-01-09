#include "Editor/PH_AssetListRequest.h"

//#include <OgreResourceGroupManager.h>

Phobos::Editor::AssetListRequest_c::AssetListRequest_c(const rapidjson::Value &value):
	Request_c(value)
{
	//empty
}


void Phobos::Editor::AssetListRequest_c::OnExecute()
{
	//Ogre::StringVectorPtr pList = Ogre::ResourceGroupManager::getSingleton().findResourceNames("ProjectResources","*.mesh",false);
}
