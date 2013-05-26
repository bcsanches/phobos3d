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

#ifndef PH_GAME_COMPONENTS_FACTORY_H
#define PH_GAME_COMPONENTS_FACTORY_H

#include <Phobos/GenericFactory.h>
#include <Phobos/Memory.h>

#include "Phobos/Game/Things/Component.h"
#include "Phobos/Game/Things/EntityFwd.h"

namespace Phobos
{	
	namespace Game
	{
		namespace Things
		{
			class PH_GAME_API ComponentFactory: public GenericFactory2<ObjectCreator2<Component, String_t, Phobos::Game::Things::Entity &, ComponentFactory>, String_t, Phobos::Game::Things::Entity & >
			{
				public:
					static ComponentFactory &GetInstance();			
			};
		}
	}
}

#define PH_ENTITY_COMPONENT_CREATOR(NAME, TYPE)										\
	static ObjectCreator2<Phobos::Game::Things::Component, String_t, Phobos::Game::Things::Entity &, Phobos::Game::Things::ComponentFactory> TYPE##_CreatorObject_gl(NAME, &TYPE::Create);

#define PH_FULL_ENTITY_COMPONENT_CREATOR(NAME, TYPE)						\
	PH_ENTITY_COMPONENT_CREATOR(NAME, TYPE);								\
	Phobos::Game::Things::Component *TYPE::Create(const String_t &name, Phobos::Game::Things::Entity &owner)	\
	{																		\
		return PH_NEW TYPE(name, owner); 									\
	}

#endif
