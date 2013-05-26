/*
Phobos 3d
February 2011
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

#ifndef PH_GAME_COMPONENTS_COMPONENT_H
#define PH_GAME_COMPONENTS_COMPONENT_H

#include "Phobos/Game/Things/ComponentFwd.h"
#include "Phobos/Game/Things/Entity.h"
#include "Phobos/Game/Things/Thing.h"

#include "Phobos/Game/GameAPI.h"

#include <Phobos/Register/Table.h>

namespace Phobos
{
	namespace Game
	{
		namespace Things
		{
			class PH_GAME_API Component: public Thing
			{
				public:
					void Load(const Register::Table &table);

					void LoadFinished();

					inline Handle_s GetEntityHandle() const;

					inline const String_t &GetEntityName() const;

				protected:
					Component(const String_t &name, Phobos::Game::Things::Entity &owner);

					virtual void OnLoad(const Register::Table &table) {};
					virtual void OnLoadFinished() {};

					template <typename T>
					inline T &GetCustomEntityProperty(const char *name)
					{
						return m_rclEntity.GetCustomProperty<T>(name);
					}

				private:
					Phobos::Game::Things::Entity &m_rclEntity;
			};

			inline Handle_s Component::GetEntityHandle() const
			{
				return m_rclEntity.GetHandle();
			}

			inline const String_t &Component::GetEntityName() const
			{
				return m_rclEntity.GetName();
			}
		}
	}
}

#endif
