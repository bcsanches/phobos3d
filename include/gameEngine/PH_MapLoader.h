/*
Phobos 3d
April 2012
Copyright (c) 2005-2011 Bruno Sanches  http://code.google.com/p/phobos3d

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef PH_MAP_LOADER_H
#define PH_MAP_LOADER_H

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <PH_String.h>
#include <PH_DictionaryHive.h>

#include "PH_Entity.h"

namespace Phobos
{		
	class MapLoader_c: boost::noncopyable
	{
		public:
			virtual void Load(const String_c &fileName) = 0;

			static void OnBoot();
				
			const DictionaryHive_c &GetStaticEntitiesHive() const;
			const DictionaryHive_c &GetDynamicEntitiesHive() const ;
			const DictionaryHive_c &GetCurrentLevelHive() const;

			EntityPtr_t CreateAndLoadWorldSpawn();

		protected:
			static void ClearAllHives();

			MapLoader_c(const Dictionary_c &settings);

			DictionaryPtr_t CreateWorldSpawnEntityDictionary();

		protected:			
			static DictionaryHivePtr_t ipStaticEntitiesHive_g;
			static DictionaryHivePtr_t ipDynamicEntitiesHive_g;
			static DictionaryHivePtr_t ipCurrentLevelHive_g;

			String_c strWorldSpawnEntityType;
	};	

	typedef boost::shared_ptr<MapLoader_c> MapLoaderPtr_t;
}

#endif
