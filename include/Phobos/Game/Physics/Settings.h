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

#ifndef PH_GAME_PHYSICS_SETTINGS_H
#define PH_GAME_PHYSICS_SETTINGS_H

#include <Phobos/String.h>
#include <Phobos/Types.h>

#include <Phobos/Register/TableFwd.h>

#include "CollisionTag.h"

#include "RigidBodyFwd.h"

namespace Phobos
{		
	namespace Game
	{
		namespace Physics
		{
			class Settings
			{
				public:
					static void OnBoot();

					static UInt32_t DecodeCollisionMask(const String_t &config);
					static Physics::CollisionTag LoadCollisionTag(const Phobos::Register::Table &table);

					static Physics::CollisionTag CreateStaticWorldCollisionTag();

					static const Phobos::Register::Table *TryGetStaticMeshCollisionShapeDef(const String_t &name);
			};
		}
	}
}


#endif
