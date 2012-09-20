#include "PH_TileMapLoader.h"

#include <PH_DictionaryManager.h>

#include "PH_MapLoaderFactory.h"
#include "PH_TileGameWorld.h"

namespace Phobos
{
	PH_FULL_MAP_LOADER_CREATOR("TileMapLoader", TileMapLoader_c);

	TileMapLoader_c::TileMapLoader_c(const Dictionary_c &settings):
		MapLoader_c(settings)
	{
		//empty
	}

	GameWorldPtr_t TileMapLoader_c::CreateGameWorld()
	{
		return boost::make_shared<TileGameWorld_c>();
	}
	
	void TileMapLoader_c::Load(const String_c &fileName)
	{
		MapLoader_c::ClearAllHives();

		DictionaryManager_c::GetInstance().Load(fileName);
	}
}

