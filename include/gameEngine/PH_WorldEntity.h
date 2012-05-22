/*
Phobos 3d
September 2011
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

#ifndef PH_WORLD_ENTITY_H
#define PH_WORLD_ENTITY_H

#include "PH_Entity.h"
#include "PH_GameEngineAPI.h"

#define PH_WORLD_SCENE_MANAGER_NAME "SceneManager"

namespace Phobos
{
	PH_DECLARE_NODE_PTR(WorldEntity);

	class Dictionary_c;
	class MapLoader_c;	

	class PH_GAME_ENGINE_API WorldEntity_c: public Entity_c
	{
		public:			
			virtual void LoadMap(const MapLoader_c &loader) = 0;
		
			WorldEntity_c(const String_c &name);
			~WorldEntity_c();			
	};
}

#endif
