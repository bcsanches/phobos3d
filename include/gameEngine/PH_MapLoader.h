/*
Phobos 3d
  September 2010

  Copyright (C) 2005-2010 Bruno Crivelari Sanches

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Bruno Crivelari Sanches bcsanches@gmail.com
*/
#ifndef PH_MAP_LOADER_H
#define PH_MAP_LOADER_H

#include <boost/noncopyable.hpp>

#include <PH_String.h>
#include <PH_DictionaryHive.h>

namespace Phobos
{
	class MapLoader_c: boost::noncopyable
	{
		public:
			void LoadOgitor(const String_c &fileName);

			void OnBoot();

			inline DictionaryHivePtr_t GetStaticEntitiesHive() const;
			inline DictionaryHivePtr_t GetDynamicEntitiesHive() const;

		private:
			String_c strCaelumDir;

			DictionaryHivePtr_t ipStaticEntitiesHive;
			DictionaryHivePtr_t	ipDynamicEntitiesHive;
	};

	inline DictionaryHivePtr_t MapLoader_c::GetStaticEntitiesHive() const
	{
		return ipStaticEntitiesHive;
	}

	inline DictionaryHivePtr_t MapLoader_c::GetDynamicEntitiesHive() const
	{
		return ipDynamicEntitiesHive;
	}
}

#endif