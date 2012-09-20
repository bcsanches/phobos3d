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

#ifndef PH_ENTITY_H
#define PH_ENTITY_H

#include "PH_EntityIO.h"
#include "PH_GameEngineAPI.h"
#include "PH_HandleManager.h"

namespace Phobos
{
	class Dictionary_c;
	class EntityComponent_c;

	PH_DECLARE_NODE_PTR(Entity);

	class PH_GAME_ENGINE_API Entity_c: public EntityIO_c
	{
		public:
			static Entity_c *Create(const String_c &name);

			void Load(const Dictionary_c &dictionary);
			void LoadFinished();

			inline const String_c &GetEntityClassName() const;

			inline void SetHandle(Handle_s handle);
			const Handle_s GetHandle() const;

		protected:
			Entity_c(const String_c &name);

			virtual void OnLoad(const Dictionary_c &dictionary) {};

			virtual void OnLoadFinished() {};

			EntityComponent_c &GetComponent(const char *typeName);

			inline const Dictionary_c &GetDictionary() const;

		private:			
			String_c			strClassName;
			const Dictionary_c	*pclDictionary;

			Handle_s hHandle;
	};

	inline const String_c &Entity_c::GetEntityClassName() const
	{
		return strClassName;
	}

	inline void Entity_c::SetHandle(Handle_s handle)
	{
		hHandle = handle;
	}

	inline const Handle_s Entity_c::GetHandle() const
	{
		return hHandle;
	}

	inline const Dictionary_c &Entity_c::GetDictionary() const
	{
		return *pclDictionary;
	}
}

#endif
