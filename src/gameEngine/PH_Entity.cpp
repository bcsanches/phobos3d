/*
Phobos 3d
  September 2010

  Copyright (C) 2005-2010 Bruno Crivelari Sanches

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Bruno Crivelari Sanches bcsanches@gmail.com
*/

#include "PH_Entity.h"
#include "PH_EntityComponentFactory.h"
#include "PH_EntityFactory.h"
#include "PH_EntityKeys.h"

#include <PH_Dictionary.h>

namespace Phobos
{
	PH_FULL_ENTITY_CREATOR("Entity", Entity_c);

	Entity_c::Entity_c(const String_c &name):
		EntityIO_c(name, PRIVATE_CHILDREN)
	{
	}

	void Entity_c::Load(const Dictionary_c &dict)
	{
		strClassName = dict.GetInherited()->GetName();

		const String_c *components = dict.TryGetValue(PH_ENTITY_KEY_COMPONENTS);
		if(components)
		{
			EntityComponentFactory_c &factory = EntityComponentFactory_c::GetInstance();

			String_c componentName;
			size_t pos = 0;
			while(StringSplitBy(componentName, *components, '|', pos, &pos))
			{
				this->AddPrivateChild(factory.Create(componentName, componentName));
			}
		}

		this->OnLoad(dict);

		for(NodeMap_t::const_iterator it = this->begin(), end = this->end(); it != end; ++it)
		{
			EntityComponentPtr_t component = boost::static_pointer_cast<EntityComponent_c>(it->second);

			component->Load(dict);
		}
	}

	void Entity_c::LoadFinished()
	{
		this->OnLoadFinished();

		for(NodeMap_t::iterator it = this->begin(), end = this->end(); it != end; ++it)
		{
			EntityComponentPtr_t component = boost::static_pointer_cast<EntityComponent_c>(it->second);

			component->LoadFinished();
		}
	}
}
