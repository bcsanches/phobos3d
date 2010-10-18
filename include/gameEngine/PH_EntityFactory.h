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

#ifndef PH_ENTITY_FACTORY_H
#define PH_ENTITY_FACTORY_H

#include <boost/noncopyable.hpp>
#include <boost/intrusive/set.hpp>

#include <PH_Defs.h>

#include "PH_Entity.h"
#include "PH_GameEngineAPI.h"

namespace Phobos
{
	typedef boost::intrusive::set_base_hook<boost::intrusive::link_mode<boost::intrusive::auto_unlink> > EntityCreatorAutoUnlinkHook_t;

	typedef EntityPtr_t (*EntityCreatorProc_t)(const String_c &name);

	class EntityCreator_c: public EntityCreatorAutoUnlinkHook_t
	{
		public:
			typedef EntityPtr_t (*EntityCreatorProc_t)(const String_c &name);

		public:
			EntityCreator_c(const String_c &name, EntityCreatorProc_t proc);

			EntityPtr_t Create(const String_c &name) const;

			inline const String_c &GetName() const
			{
				return strName;
			}

			inline bool operator<(const EntityCreator_c &rhs) const
            {
                return strName.compare(rhs.strName) < 0;    
            }

		private:
			String_c strName;
			EntityCreatorProc_t pfnCreateProc;
	};

	class PH_GAME_ENGINE_API EntityFactory_c: boost::noncopyable
	{
		public:
			static EntityFactory_c &GetInstance();

			EntityPtr_t Create(const String_c &className, const String_c &name) const;			

		private:
			EntityFactory_c();

			friend class EntityCreator_c;
			void Register(EntityCreator_c &creator);

			static EntityFactory_c clInstance_gl;

		private:
			typedef boost::intrusive::set<EntityCreator_c, boost::intrusive::constant_time_size<false> > EntityCreatorSet_t;
            EntityCreatorSet_t setEntitiesCreator;
	};
}

#define PH_ENTITY_CREATOR(NAME, TYPE)											\
	static EntityCreator_c TYPE##_CreatorObject_gl(NAME, TYPE::Create);

#define PH_FULL_ENTITY_CREATOR(NAME, TYPE)			\
	PH_ENTITY_CREATOR(NAME, TYPE);					\
	EntityPtr_t TYPE::Create(const String_c &name)	\
	{												\
		return new TYPE(name);						\
	}

#endif
