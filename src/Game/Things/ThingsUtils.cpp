/*
Phobos 3d
July 2012
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

#include <Phobos/Register/Table.h>

#include <Phobos/OgreEngine/Math/Transform.h>

#include "Phobos/Game/Level/MapDefs.h"
#include "Phobos/Game/Things/Keys.h"
#include "Phobos/Game/RegisterUtils.h"
#include "Phobos/Game/WorldManager.h"

namespace Phobos
{
	namespace Game
	{
		namespace Things
		{
			void LoadWorldTransform(Engine::Math::Transform &transform, const Phobos::Register::Table &table)
			{
				transform.SetOrigin(Register::GetVector3(table, PH_MAP_OBJECT_KEY_WORLD_POSITION));
				transform.SetRotation(Register::GetQuaternion(table, PH_MAP_OBJECT_KEY_WORLD_ORIENTATION));
			}

			void SaveWorldTransform(Phobos::Register::Table &table, const Engine::Math::Transform &transform)
			{
				Register::SetVector3(table, PH_MAP_OBJECT_KEY_WORLD_POSITION, transform.GetOrigin());
				Register::SetQuaternion(table, PH_MAP_OBJECT_KEY_WORLD_ORIENTATION, transform.GetRotation());
			}

			void SaveTransform(Phobos::Register::Table &table, const Engine::Math::Transform &transform)
			{
				Register::SetVector3(table, PH_MAP_OBJECT_KEY_POSITION, transform.GetOrigin());
				Register::SetQuaternion(table, PH_MAP_OBJECT_KEY_ORIENTATION, transform.GetRotation());
			}
		}
	}
}
