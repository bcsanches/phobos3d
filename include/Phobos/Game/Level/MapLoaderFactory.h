/*
Phobos 3d
April 2012
Copyright (c) 2005-2012 Bruno Sanches  http://code.google.com/p/phobos3d

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef PH_MAP_LOADER_FACTORY_H
#define PH_MAP_LOADER_FACTORY_H

#include <Phobos/GenericFactory.h>
#include <Phobos/Memory.h>

#include <Phobos/Register/HiveFwd.h>

#include "Phobos/Game/GameAPI.h"
#include "Phobos/Game/Level/MapLoader.h"

#include <list>

namespace Phobos
{
	namespace Game
	{
		class PH_GAME_API MapLoaderFactory 
		{
			public:
				static MapLoaderFactory &GetInstance();			

				MapLoaderPtr_t Create(const String_t &type);

				void Register(Phobos::ObjectCreator1<Phobos::Game::MapLoader, Register::Table, Phobos::Game::MapLoaderFactory> &creator)
				{
					m_clFactory.Register(creator);
				}

				inline const Register::Hive &GetStaticEntitiesHive() const;
				inline const Register::Hive &GetDynamicEntitiesHive() const;		

				std::list<String_t> CreateMapFileExtensionsList() const;

			private:
				MapLoaderFactory();

			private:
				Register::HivePtr_t m_ipStaticEntitiesHive;
				Register::HivePtr_t	m_ipDynamicEntitiesHive;
				Register::HivePtr_t m_ipCurrentLevelHive;

				GenericFactory1<ObjectCreator1<MapLoader, Register::Table, MapLoaderFactory>, Register::Table > m_clFactory;			
		};

		inline const Register::Hive &MapLoaderFactory::GetStaticEntitiesHive() const
		{
			return *m_ipStaticEntitiesHive;
		}

		inline const Register::Hive &MapLoaderFactory::GetDynamicEntitiesHive() const
		{
			return *m_ipDynamicEntitiesHive;
		}
	}
}

#define PH_MAP_LOADER_CREATOR(NAME, TYPE)										\
	static Phobos::ObjectCreator1<Phobos::Game::MapLoader, Register::Table, Phobos::Game::MapLoaderFactory> TYPE##_CreatorObject_gl(NAME, TYPE::Create);

#define PH_FULL_MAP_LOADER_CREATOR(NAME, TYPE)  							\
	PH_MAP_LOADER_CREATOR(NAME, TYPE);										\
	Phobos::Game::MapLoader *TYPE::Create(const Register::Table &settings)		\
	{																		\
		return PH_NEW TYPE(settings);										\
	}

#endif
