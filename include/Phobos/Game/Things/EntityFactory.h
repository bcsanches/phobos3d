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

#ifndef PH_ENTITY_FACTORY_H
#define PH_ENTITY_FACTORY_H

#include <memory>

#include <Phobos/GenericFactory.h>
#include <Phobos/Memory.h>

#include "Phobos/Game/Things/Entity.h"

namespace Phobos
{	
	namespace Game
	{
		namespace Things
		{
			class PH_GAME_API EntityFactory: public GenericFactory1<ObjectCreator1<Entity, String_t, EntityFactory, std::unique_ptr<Entity>>, String_t >
			{
				public:
					static EntityFactory &GetInstance();			
			};
		}
	}
}

#define PH_ENTITY_CREATOR(NAME, TYPE)										\
	static Phobos::ObjectCreator1<Phobos::Game::Things::Entity, Phobos::String_t, Phobos::Game::Things::EntityFactory, std::unique_ptr<Phobos::Game::Things::Entity>> TYPE##_CreatorObject_gl(NAME, TYPE::Create);

#define PH_FULL_ENTITY_CREATOR(NAME, TYPE)  		\
	PH_ENTITY_CREATOR(NAME, TYPE);					\
	std::unique_ptr<Phobos::Game::Things::Entity> TYPE::Create(const Phobos::String_t &name)	\
	{												\
		return std::unique_ptr<Phobos::Game::Things::Entity>(new TYPE(name));					\
	}

#endif
