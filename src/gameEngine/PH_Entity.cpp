/*
Phobos 3d
September 2011
Copyright (c) 2005-2013 Bruno Sanches  http://code.google.com/p/phobos3d

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

#include <Phobos/Register/Table.h>

namespace Phobos
{
	PH_FULL_ENTITY_CREATOR("Entity", Entity);

	Entity::Entity(const String_t &name):
		EntityIO(name, NodeFlags::PRIVATE_CHILDREN),
		m_pclTable(NULL)
	{
	}

	void Entity::Load(const Register::Table &table)
	{
		m_pclTable = &table;

		m_strClassName = table.GetInherited()->GetName();

		const String_t *components = table.TryGetString(PH_ENTITY_KEY_COMPONENTS);
		if(components)
		{
			EntityComponentFactory &factory = EntityComponentFactory::GetInstance();

			String_t componentName;
			size_t pos = 0;
			
			while(StringSplitBy(componentName, *components, '|', pos, &pos))
			{				
				std::unique_ptr<EntityComponent> comp(factory.Create(componentName, componentName, *this));
				
				this->AddPrivateChild(std::move(comp));
			}
		}

		this->OnLoad(table);

		for(NodeMap_t::iterator it = this->begin(), end = this->end(); it != end; ++it)
		{
			EntityComponent &component = static_cast<EntityComponent &>(*it->second);

			component.Load(table);
		}
	}

	void Entity::LoadFinished()
	{
		this->OnLoadFinished();

		for(NodeMap_t::iterator it = this->begin(), end = this->end(); it != end; ++it)
		{
			EntityComponent &component = static_cast<EntityComponent &>(*it->second);

			component.LoadFinished();
		}
	}

	EntityComponent &Entity::GetComponent(const char *typeName)
	{
		return static_cast<EntityComponent &>(this->GetChild(typeName));			
	}
}
