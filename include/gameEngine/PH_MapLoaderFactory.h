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

#include <boost/make_shared.hpp>

#include <PH_GenericFactory.h>

#include "PH_GameEngineAPI.h"
#include "PH_MapLoader.h"

namespace Phobos
{
	class PH_GAME_ENGINE_API MapLoaderFactory_c 
	{
		public:
			static MapLoaderFactory_c &GetInstance();			

			MapLoaderPtr_t Create(const String_c &type);

			void Register(Phobos::ObjectCreator1_c<Phobos::MapLoader_c, Dictionary_c, Phobos::MapLoaderFactory_c> &creator)
			{
				clFactory.Register(creator);
			}

			inline const DictionaryHive_c &GetStaticEntitiesHive() const;
			inline const DictionaryHive_c &GetDynamicEntitiesHive() const;		

		private:
			MapLoaderFactory_c();

		private:
			DictionaryHivePtr_t ipStaticEntitiesHive;
			DictionaryHivePtr_t	ipDynamicEntitiesHive;
			DictionaryHivePtr_t ipCurrentLevelHive;

			GenericFactory1_c<ObjectCreator1_c<MapLoader_c, Dictionary_c, MapLoaderFactory_c>, Dictionary_c > clFactory;			
	};

	inline const DictionaryHive_c &MapLoaderFactory_c::GetStaticEntitiesHive() const
	{
		return *ipStaticEntitiesHive;
	}

	inline const DictionaryHive_c &MapLoaderFactory_c::GetDynamicEntitiesHive() const
	{
		return *ipDynamicEntitiesHive;
	}
}

#define PH_MAP_LOADER_CREATOR(NAME, TYPE)										\
	static Phobos::ObjectCreator1_c<Phobos::MapLoader_c, Dictionary_c, Phobos::MapLoaderFactory_c> TYPE##_CreatorObject_gl(NAME, TYPE::Create);

#define PH_FULL_MAP_LOADER_CREATOR(NAME, TYPE)  							\
	PH_MAP_LOADER_CREATOR(NAME, TYPE);										\
	Phobos::MapLoader_c *TYPE::Create(const Dictionary_c &settings)			\
	{																		\
		return PH_NEW TYPE(settings);										\
	}

#endif
