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

#ifndef PH_ENTITY_H
#define PH_ENTITY_H

#include <Phobos/Register/TableFwd.h>

#include "Phobos/Game/GameAPI.h"

#include "Phobos/Game/Things/Thing.h"
#include "Phobos/Game/Things/HandleManager.h"
#include "Phobos/Game/Things/ComponentFwd.h"

namespace Phobos
{	
	namespace Game
	{
		namespace Things
		{	
			PH_DECLARE_NODE_PTR(Entity);

			class PH_GAME_API Entity: public Thing
			{
				public:
					static std::unique_ptr<Entity> Create(const String_t &name);

					void Load(const Register::Table &table);
					void LoadFinished();

					inline const String_t &GetEntityClassName() const;

					inline void SetHandle(Handle_s handle);
					const Handle_s GetHandle() const;

				protected:
					Entity(const String_t &name);

					virtual void OnLoad(const Register::Table &table) {};

					virtual void OnLoadFinished() {};

					Things::Component &GetComponent(const char *typeName);

					inline const Register::Table &GetTable() const;

				private:			
					String_t				m_strClassName;
					const Register::Table	*m_pclTable;

					Handle_s m_hHandle;
			};

			inline const String_t &Entity::GetEntityClassName() const
			{
				return m_strClassName;
			}

			inline void Entity::SetHandle(Handle_s handle)
			{
				m_hHandle = handle;
			}

			inline const Handle_s Entity::GetHandle() const
			{
				return m_hHandle;
			}

			inline const Register::Table &Entity::GetTable() const
			{
				return *m_pclTable;
			}
		}
	}
}

#endif
