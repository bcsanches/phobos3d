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

#include "PH_EntityFactory.h"
#include "PH_Exception.h"

namespace Phobos
{
	EntityFactory_c EntityFactory_c::clInstance_gl;

	struct EntityCreatorComp_s
    {
        bool operator()(const String_c &name, const EntityCreator_c &res) const
        {
            return name.compare(res.GetName()) < 0;    
        }
 
        bool operator()(const EntityCreator_c &res, const String_c &name) const
        {
            return res.GetName().compare(name) < 0;
        }
    };

	EntityCreator_c::EntityCreator_c(const String_c &name, EntityCreatorProc_t proc):
		strName(name),
		pfnCreateProc(proc)
	{
		EntityFactory_c::GetInstance().Register(*this);

		if(proc == NULL)
		{
			std::stringstream stream;
			stream << "creator proc cant be null, entity " << name;
			PH_RAISE(INVALID_PARAMETER_EXCEPTION, "[EntityCreator_c::EntityCreator_c]", stream.str());
		}
	}

	EntityPtr_t EntityCreator_c::Create(const String_c &name) const
	{
		return pfnCreateProc(name);
	}

	EntityFactory_c &EntityFactory_c::GetInstance()
	{
		return clInstance_gl;
	}

	EntityFactory_c::EntityFactory_c()
	{
		//empty
	}

	void EntityFactory_c::Register(EntityCreator_c &creator)
	{
		setEntitiesCreator.insert(creator);
	}

	EntityPtr_t EntityFactory_c::Create(const String_c &className, const String_c &name) const
	{
		EntityCreatorSet_t::const_iterator it = setEntitiesCreator.find(className, EntityCreatorComp_s());
		if(it == setEntitiesCreator.end())
			PH_RAISE(OBJECT_NOT_FOUND_EXCEPTION, "[EntityFactory_c::Create]", name);

		return it->Create(name);
	}
}


