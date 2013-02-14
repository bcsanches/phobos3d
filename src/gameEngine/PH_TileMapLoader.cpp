#include "PH_TileMapLoader.h"

#include <Phobos/Register/Manager.h>

#include "PH_MapLoaderFactory.h"
#include "PH_TileGameWorld.h"

namespace Phobos
{
	PH_FULL_MAP_LOADER_CREATOR("TileMapLoader", TileMapLoader);

	TileMapLoader::TileMapLoader(const Register::Table &settings):
		MapLoader(settings)
	{
		//empty
	}

	GameWorldPtr_t TileMapLoader::CreateGameWorld()
	{
		return std::make_shared<TileGameWorld>();
	}
	
	void TileMapLoader::Load(const String_t &fileName)
	{
		MapLoader::ClearAllHives();

		Register::Load(fileName);
	}
}

