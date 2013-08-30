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

#include <Phobos/DisableCopy.h>
#include <Phobos/String.h>

#include <Phobos/Register/HiveFwd.h>
#include <Phobos/Register/TableFwd.h>

namespace Phobos
{		
	namespace Game
	{
		class MapLoader
		{
			public:
				void Load(StringRef_t fileName, Register::Hive &gameObjectsHive);

				void Unload();						

			protected:				
				virtual void OnLoad(StringRef_t fileName, Register::Hive &gameObjectsHive) = 0;

				MapLoader(const Phobos::Register::Table &settings);				
							
			private:											
				String_t m_strWorldSpawnEntityType;

				PH_DISABLE_COPY(MapLoader);
		};	

		typedef std::shared_ptr<MapLoader> MapLoaderPtr_t;
	}
}

#endif
