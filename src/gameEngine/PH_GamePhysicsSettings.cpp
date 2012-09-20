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

#include "PH_GamePhysicsSettings.h"

#include <PH_Dictionary.h>
#include <PH_DictionaryHive.h>
#include <PH_DictionaryManager.h>

#include "PH_PhysicsKeys.h"

namespace Phobos
{
	static Dictionary_c *pclCollisionGroups_gl;
	static Dictionary_c *pclStaticWorldCollision_gl;
	static DictionaryHive_c *pclStaticMeshCollisionShapeDefHive_gl;

	void GamePhysicsSettings_c::OnBoot()
	{
		DictionaryManager_c &dictManager = DictionaryManager_c::GetInstance();

		pclCollisionGroups_gl = &dictManager.GetDictionary(PH_PHYSICS_DEF, PH_DICTIONARY_COLLISION_GROUP);
		pclStaticWorldCollision_gl = &dictManager.GetDictionary(PH_PHYSICS_DEF, PH_DICTIONARY_STATIC_WORLD_COLLISION);
		pclStaticMeshCollisionShapeDefHive_gl = dictManager.TryGetDictionaryHive("StaticMeshCollisionShapeDef");					
	}

	UInt32_t GamePhysicsSettings_c::DecodeCollisionMask(const String_c &config)
	{
		UInt32_t flags = 0;

		String_c stringFlag;
		size_t pos = 0;
		while(StringSplitBy(stringFlag, config, '|', pos, &pos))
		{
			StringTrim(stringFlag, STRING_TRIM_BOTH);

			flags |= pclCollisionGroups_gl->GetInt(stringFlag);
		}

		return flags;
	}

	Physics::CollisionTag_c GamePhysicsSettings_c::LoadCollisionTag(const Dictionary_c &dict)
	{
		UInt32_t group = GamePhysicsSettings_c::DecodeCollisionMask(dict.GetString(PH_PHYSICS_KEY_COLLSION_GROUP));
		UInt32_t filter = DecodeCollisionMask(dict.GetString(PH_PHYSICS_KEY_COLLSION_FILTER));

		return Physics::CollisionTag_c(group, filter);
	}

	Physics::CollisionTag_c GamePhysicsSettings_c::CreateStaticWorldCollisionTag()
	{
		return GamePhysicsSettings_c::LoadCollisionTag(*pclStaticWorldCollision_gl);
	}

	const Dictionary_c *GamePhysicsSettings_c::TryGetStaticMeshCollisionShapeDef(const String_c &name)
	{
		return pclStaticMeshCollisionShapeDefHive_gl ? pclStaticMeshCollisionShapeDefHive_gl->TryGetDictionary(name) : NULL;
	}
}
