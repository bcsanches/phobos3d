#include "Phobos/Game/MapDefs.h"

#include <Phobos/Exception.h>

#include <tuple>

namespace Phobos
{
	namespace Game
	{		
		static std::tuple<MapObjectTypes, const char *> g_artplTypes[] = {
			std::make_tuple(MapObjectTypes::TERRAIN_PAGE,	PH_MAP_OBJECT_TYPE_TERRAIN_PAGE),
			std::make_tuple(MapObjectTypes::TERRAIN,		PH_MAP_OBJECT_TYPE_TERRAIN),
			std::make_tuple(MapObjectTypes::STATIC,		PH_MAP_OBJECT_TYPE_STATIC),
			std::make_tuple(MapObjectTypes::ENTITY,		PH_MAP_OBJECT_TYPE_ENTITY),
			std::make_tuple(MapObjectTypes::STATIC_LIGHT,	PH_MAP_OBJECT_TYPE_STATIC_LIGHT),
			std::make_tuple(MapObjectTypes::SCENE_MANAGER,	PH_MAP_OBJECT_TYPE_SCENE_MANAGER),

			//sentinel
			std::make_tuple(MapObjectTypes::TERRAIN_PAGE, nullptr)
		};

		const char *MapObjectTypeToString(MapObjectTypes type)
		{
			return std::get<1>(g_artplTypes[static_cast<int>(type)]);
		}

#ifdef PH_MSVC
	//warning C4297: 'Phobos::Game::StringToMapObjectType' : function assumed not to throw an exception but does
	#pragma warning (disable : 4297)
#endif

		MapObjectTypes StringToMapObjectType(const char *type)
		{
			for (int i = 0; std::get<1>(g_artplTypes[i]); ++i)
			{
				if (strcmp(std::get<1>(g_artplTypes[i]), type) == 0)
					return std::get<0>(g_artplTypes[i]);
			}

			std::stringstream stream;
			stream << "Invalid value " << type << " for StringToMapObjectType";
			PH_RAISE(INVALID_PARAMETER_EXCEPTION, "Phobos::Game::MapDefs", stream.str());
		}
	}
}
