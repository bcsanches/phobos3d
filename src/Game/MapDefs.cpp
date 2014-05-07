#include "Phobos/Game/MapDefs.h"

#include <Phobos/Exception.h>

#include <tuple>

namespace Phobos
{
	namespace Game
	{	
		template <typename T, typename ENUM>
		const char *TypeToString(T tuples, ENUM type)
		{
			return std::get<1>(tuples[static_cast<int>(type)]);
		}

		template <typename ENUM, typename T>
		ENUM StringToType(T tuples, const char *type)
		{
			for (int i = 0; std::get<1>(tuples[i]); ++i)
			{
				if (strcmp(std::get<1>(tuples[i]), type) == 0)
					return std::get<0>(tuples[i]);
			}

			std::stringstream stream;
			stream << "Invalid value " << type << " for StringToType";
			PH_RAISE(INVALID_PARAMETER_EXCEPTION, "Phobos::Game::MapDefs", stream.str());
		}

		static std::tuple<MapObjectTypes, const char *> g_artplTypes[] = {
			std::make_tuple(MapObjectTypes::TERRAIN_PAGE,	PH_MAP_OBJECT_TYPE_TERRAIN_PAGE),
			std::make_tuple(MapObjectTypes::TERRAIN,		PH_MAP_OBJECT_TYPE_TERRAIN),
			std::make_tuple(MapObjectTypes::STATIC,			PH_MAP_OBJECT_TYPE_STATIC),
			std::make_tuple(MapObjectTypes::ENTITY,			PH_MAP_OBJECT_TYPE_ENTITY),
			std::make_tuple(MapObjectTypes::STATIC_LIGHT,	PH_MAP_OBJECT_TYPE_STATIC_LIGHT),
			std::make_tuple(MapObjectTypes::SCENE_MANAGER,	PH_MAP_OBJECT_TYPE_SCENE_MANAGER),

			//sentinel
			std::make_tuple(MapObjectTypes::TERRAIN_PAGE, nullptr)
		};
		
		const char *MapObjectTypeToString(MapObjectTypes type)
		{			
			return TypeToString(g_artplTypes, type);
		}

#ifdef PH_MSVC
	//warning C4297: 'Phobos::Game::StringToMapObjectType' : function assumed not to throw an exception but does
	#pragma warning (disable : 4297)
#endif

		MapObjectTypes StringToMapObjectType(const char *type)
		{
			return StringToType<MapObjectTypes>(g_artplTypes, type);
		}

		static std::tuple<PhysicsTypes, const char *> g_artplPhysicsTypes[] = {
			std::make_tuple(PhysicsTypes::NONE, PH_MAP_OBJECT_PHYSICS_TYPE_NONE),
			std::make_tuple(PhysicsTypes::DYNAMIC, PH_MAP_OBJECT_PHYSICS_TYPE_DYNAMIC),
			std::make_tuple(PhysicsTypes::KINEMATIC, PH_MAP_OBJECT_PHYSICS_TYPE_KINEMATIC),
			std::make_tuple(PhysicsTypes::STATIC, PH_MAP_OBJECT_PHYSICS_TYPE_STATIC),

			//sentinel
			std::make_tuple(PhysicsTypes::NONE, nullptr)
		};


		PH_GAME_API_PROC const char *PhysicsTypeToString(PhysicsTypes type)
		{
			return TypeToString(g_artplPhysicsTypes, type);
		}

		PH_GAME_API_PROC PhysicsTypes StringToPhysicsType(const char *type)
		{
			return StringToType<PhysicsTypes>(g_artplPhysicsTypes, type);
		}
	}
}
