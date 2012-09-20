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

#include "PH_Entity.h"
#include "PH_EntityComponentFactory.h"
#include "PH_EntityFactory.h"
#include "PH_EntityKeys.h"

#include <PH_Dictionary.h>

namespace Phobos
{
	PH_FULL_ENTITY_CREATOR("Entity", Entity_c);

	Entity_c::Entity_c(const String_c &name):
		EntityIO_c(name, NodeFlags::PRIVATE_CHILDREN),
		pclDictionary(NULL)
	{
	}

	void Entity_c::Load(const Dictionary_c &dict)
	{
		pclDictionary = &dict;

		strClassName = dict.GetInherited()->GetName();

		const String_c *components = dict.TryGetString(PH_ENTITY_KEY_COMPONENTS);
		if(components)
		{
			EntityComponentFactory_c &factory = EntityComponentFactory_c::GetInstance();

			String_c componentName;
			size_t pos = 0;
			
			while(StringSplitBy(componentName, *components, '|', pos, &pos))
			{				
				std::auto_ptr<EntityComponent_c> comp(factory.Create(componentName, componentName, *this));

				comp->SetManaged(true);

				this->AddPrivateChild(*comp);

				comp.release();
			}
		}

		this->OnLoad(dict);

		for(NodeMap_t::iterator it = this->begin(), end = this->end(); it != end; ++it)
		{
			EntityComponent_c &component = static_cast<EntityComponent_c &>(*it->second);

			component.Load(dict);
		}
	}

	void Entity_c::LoadFinished()
	{
		this->OnLoadFinished();

		for(NodeMap_t::iterator it = this->begin(), end = this->end(); it != end; ++it)
		{
			EntityComponent_c &component = static_cast<EntityComponent_c &>(*it->second);

			component.LoadFinished();
		}
	}

	EntityComponent_c &Entity_c::GetComponent(const char *typeName)
	{
		return static_cast<EntityComponent_c &>(this->GetChild(typeName));			
	}
}
