/*
Phobos 3d
May 2012
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

#ifndef PH_GAME_WORLD_H
#define PH_GAME_WORLD_H

#include <Phobos/Register/TableFwd.h>

#include "GameAPI.h"
#include "WorldFwd.h"

namespace Phobos
{		
	namespace Game
	{
		class MapLoader;			

		enum PhysicsCollisionGroups
		{
			PCG_STATIC	= 0x0001,
			PCG_DYNAMIC = 0x0002,
			PCG_CHARACTER = 0x0004,
			PCG_PLAYER_CHARACTER = 0x0008
		};

		class PH_GAME_API World
		{
			public:			
				virtual void Load(const MapLoader &loader, const Register::Table &worldEntityTable) = 0;
		
			protected:
				World() {};
				virtual ~World() {};			
		};
	}
}

#endif
