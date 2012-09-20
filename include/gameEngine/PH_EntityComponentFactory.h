/*
Phobos 3d
February 2011
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

#ifndef PH_ENTITY_COMPONENT_FACTORY_H
#define PH_ENTITY_COMPONENT_FACTORY_H

#include <PH_GenericFactory.h>
#include <PH_Memory.h>

#include "PH_EntityComponent.h"

namespace Phobos
{
	class Entity_c;

	//typedef GenericFactory_c<EntityComponentPtr_t> EntityComponentFactory_c;
	//typedef GenericFactory1_c<ObjectCreator1_c<EntityComponentPtr_t, Entity_c &>, Entity_c & > EntityComponentFactory_c;
	class PH_GAME_ENGINE_API EntityComponentFactory_c: public GenericFactory2_c<ObjectCreator2_c<EntityComponent_c, String_c, Entity_c &, EntityComponentFactory_c>, String_c, Entity_c & >
	{
		public:
			static EntityComponentFactory_c &GetInstance();			
	};
}

#define PH_ENTITY_COMPONENT_CREATOR(NAME, TYPE)										\
	static ObjectCreator2_c<EntityComponent_c, String_c, Entity_c &, Phobos::EntityComponentFactory_c> TYPE##_CreatorObject_gl(NAME, &TYPE::Create);

#define PH_FULL_ENTITY_COMPONENT_CREATOR(NAME, TYPE)						\
	PH_ENTITY_COMPONENT_CREATOR(NAME, TYPE);								\
	EntityComponent_c *TYPE::Create(const String_c &name, Entity_c &owner)	\
	{																		\
		return PH_NEW TYPE(name, owner); 									\
	}

#endif
