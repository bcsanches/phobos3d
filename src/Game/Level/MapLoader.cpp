/*
Phobos 3d
September 2010
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


#include "Phobos/Game/Level/MapLoader.h"

#include <Phobos/Path.h>
#include <Phobos/Register/Table.h>
#include <Phobos/Register/Hive.h>

#include "Phobos/Game/Level/MapWorld.h"

namespace Phobos
{	
	namespace Game
	{
		void MapLoader::Load(StringRef_t fileName, Register::Hive &gameObjectsHive)
		{			
			//delegate the load processing
			this->OnLoad(fileName, gameObjectsHive);			

			Path path(fileName);
			Path filePath;

			path.ExtractPathAndFilename(&filePath, nullptr);

			//Load all rendering stuff
			MapWorld::LoadAccess::Load(filePath.GetStr(), gameObjectsHive);			
		}

		void MapLoader::Unload()
		{
			MapWorld::LoadAccess::Unload();
		}
				
		MapLoader::MapLoader(const Register::Table &settings):
			m_strWorldSpawnEntityType(settings.GetString("worldSpawnEntityDef"))
		{
			//empty
		}				
	}
}
