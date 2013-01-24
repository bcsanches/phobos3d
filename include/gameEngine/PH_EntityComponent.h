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

#ifndef PH_ENTITY_COMPONENT_H
#define PH_ENTITY_COMPONENT_H

#include "PH_Entity.h"
#include "PH_EntityIO.h"
#include "PH_GameEngineAPI.h"

#include <Phobos/Register/Table.h>

namespace Phobos
{
	PH_DECLARE_NODE_PTR(EntityComponent);

	class PH_GAME_ENGINE_API EntityComponent_c: public EntityIO_c
	{
		public:
			void Load(const Register::Table_c &table);

			void LoadFinished();

			inline Handle_s GetEntityHandle() const;

			inline const String_c &GetEntityName() const;

		protected:
			EntityComponent_c(const String_c &name, Entity_c &owner);

			virtual void OnLoad(const Register::Table_c &table) {};
			virtual void OnLoadFinished() {};

			template <typename T>
			inline T &GetCustomEntityProperty(const char *name)
			{
				return rclEntity.GetCustomProperty<T>(name);
			}

		private:
			Entity_c &rclEntity;
	};

	inline Handle_s EntityComponent_c::GetEntityHandle() const
	{
		return rclEntity.GetHandle();
	}

	inline const String_c &EntityComponent_c::GetEntityName() const
	{
		return rclEntity.GetName();
	}
}

#endif
