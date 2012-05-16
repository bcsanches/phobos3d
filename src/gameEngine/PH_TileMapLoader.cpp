#include "PH_TileMapLoader.h"

#include "PH_MapLoaderFactory.h"

#include <PH_DictionaryManager.h>

namespace Phobos
{
	PH_FULL_MAP_LOADER_CREATOR("TileMapLoader", TileMapLoader_c);

	TileMapLoader_c::TileMapLoader_c(const Dictionary_c &settings):
		MapLoader_c(settings)
	{
		//empty
	}
	
	void TileMapLoader_c::Load(const String_c &fileName)
	{
		DictionaryManager_c::GetInstance()->Load(fileName);
	}
}

