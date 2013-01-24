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

#include <PH_String.h>
#include <PH_Types.h>

#include <Phobos/Register/TableFwd.h>

#include "PH_CollisionTag.h"

#include "PH_RigidBodyFwd.h"

namespace Phobos
{		
	class GamePhysicsSettings_c
	{
		public:
			static void OnBoot();

			static UInt32_t DecodeCollisionMask(const String_c &config);
			static Physics::CollisionTag_c LoadCollisionTag(const Register::Table_c &table);

			static Physics::CollisionTag_c CreateStaticWorldCollisionTag();

			static const Register::Table_c *TryGetStaticMeshCollisionShapeDef(const String_c &name);
	};
}


#endif
