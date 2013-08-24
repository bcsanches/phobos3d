#include "Phobos/Game/TileMapLoader.h"

#include <Phobos/Register/Manager.h>

#include "Phobos/Game/MapLoaderFactory.h"
#include "Phobos/Game/TileWorld.h"

namespace Phobos
{
	namespace Game
	{
		PH_FULL_MAP_LOADER_CREATOR("TileMapLoader", TileMapLoader);

		TileMapLoader::TileMapLoader(const Register::Table &settings):
			MapLoader(settings)
		{
			//empty
		}

		WorldPtr_t TileMapLoader::CreateWorld()
		{
			return std::make_shared<TileWorld>();
		}
	
		void TileMapLoader::OnLoad(StringRef_t fileName)
		{			
			Register::Load(fileName);
		}
	}
}

