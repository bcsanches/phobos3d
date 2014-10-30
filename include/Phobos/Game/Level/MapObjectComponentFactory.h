#ifndef PH_PHOBOS_GAME_LEVEL_MAP_OBJECT_COMPONENT_FACTORY_H
#define PH_PHOBOS_GAME_LEVEL_MAP_OBJECT_COMPONENT_FACTORY_H

#include <Phobos/GenericFactory.h>
#include <Phobos/DisableCopy.h>
#include <Phobos/Register/TableFwd.h>

#include "Phobos/Game/Level/MapObjectComponent.h"


namespace Phobos
{
	namespace Game
	{
		class MapObjectComponentFactory : public GenericFactoryEx<MapObjectComponent, MapObjectComponent::UniquePtr_t, MapObject &, const Phobos::Register::Table &>
		{
			PH_DISABLE_COPY(MapObjectComponentFactory);

			public:
				static MapObjectComponentFactory &GetInstance();

			private:
				inline MapObjectComponentFactory() {};
		};
	}
}

#define PH_MAP_COMPONENT_CREATOR(NAME, PROC)											\
	static Phobos::Game::MapObjectComponentFactory::ObjectCreator_t TYPE_##CreatorObject_gl(NAME, PROC, Phobos::Game::MapObjectComponentFactory::GetInstance())	

#define PH_MAP_COMPONENT_FULL_CREATOR(NAME, TYPE)  		\
	Phobos::Game::MapObjectComponent::UniquePtr_t TYPE::Create(Phobos::Game::MapObject &owner, const Phobos::Register::Table &table)	\
{												\
	return MapObjectComponent::UniquePtr_t(std::make_unique<TYPE>(owner, table));					\
}\
PH_MAP_COMPONENT_CREATOR(NAME, &TYPE::Create);


#endif