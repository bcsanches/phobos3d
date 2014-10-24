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

#include "Phobos/Game/Things/ComponentFactory.h"

#include "Phobos/Game/Level/MapObject.h"

#include "Phobos/Game/Things/Entity.h"
#include "Phobos/Game/Things/EntityFactory.h"
#include "Phobos/Game/Things/Keys.h"

#include <Phobos/Register/Table.h>

namespace Phobos
{
	namespace Game
	{
		namespace Things
		{
			PH_FULL_ENTITY_CREATOR("Entity", Entity);

			Entity::Entity(MapObject &mapObject) :
				Thing(mapObject.GetName(), NodeFlags::PRIVATE_CHILDREN),
				m_pclTable(NULL),
				m_rclMapObject(mapObject)
			{
			}

			void Entity::Load(const Register::Table &table)
			{
				m_pclTable = &table;

				m_strClassName = table.GetInherited()->GetName();

				const String_t *components = table.TryGetString(PH_ENTITY_KEY_COMPONENTS);
				if(components)
				{
					auto &factory = Things::ComponentFactory::GetInstance();

					String_t componentName;
					size_t pos = 0;
			
					while(StringSplitBy(componentName, *components, '|', pos, &pos))
					{				
						auto comp(factory.Create(componentName, componentName, *this));
				
						this->AddPrivateChild(std::move(comp));
					}
				}

				this->OnLoad(table);

				for(NodeMap_t::iterator it = this->begin(), end = this->end(); it != end; ++it)
				{
					auto &component = static_cast<Things::Component &>(*it->second);

					component.Load(table);
				}
			}

			void Entity::LoadFinished()
			{
				this->OnLoadFinished();

				for(NodeMap_t::iterator it = this->begin(), end = this->end(); it != end; ++it)
				{
					auto &component = static_cast<Things::Component &>(*it->second);

					component.LoadFinished();
				}
			}

			Things::Component &Entity::GetComponent(const char *typeName)
			{
				return static_cast<Things::Component &>(this->GetChild(typeName));			
			}
			
			Engine::Math::Transform Entity::MakeWorldTransform() const
			{
				return m_rclMapObject.MakeWorldTransform();
			}

			void Entity::SetTransform(const Engine::Math::Transform &transform)
			{
				m_rclMapObject.SetTransform(transform);
			}
		}
	}
}
