/*
Phobos 3d
May 2011
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

#include "PH_EntityIO.h"

#include <Phobos/Exception.h>

#include "PH_GameEventManager.h"
#include "PH_WorldManager.h"

namespace Phobos
{
	EntityIO::EntityIO(const String_t &name, UInt32_t flags):
		Node(name, flags),
		fFixedUpdateEnabled(false),
		fUpdateEnabled(false)
	{
	}

	EntityIO::EntityIO(const Char_t *name, UInt32_t flags):
		Node(name, flags),
		fFixedUpdateEnabled(false),
		fUpdateEnabled(false)
	{
	}

	EntityIO::~EntityIO()
	{
		GameEventManager::GetInstance().CancelEvents(*this);		

		if(fFixedUpdateEnabled || fUpdateEnabled)
		{
			WorldManager &world = WorldManager::GetInstance();

			if(fFixedUpdateEnabled)
				world.RemoveFromFixedUpdateList(*this);

			if(fUpdateEnabled)
				world.RemoveFromUpdateList(*this);
		}
	}

	void EntityIO::FixedUpdate()
	{
		this->OnFixedUpdate();
	}

	void EntityIO::Update()
	{
		this->OnUpdate();
	}

	void EntityIO::EnableFixedUpdate()
	{
		if(fFixedUpdateEnabled)
			return;

		WorldManager::GetInstance().AddToFixedUpdateList(*this);
		fFixedUpdateEnabled = true;
	}

	void EntityIO::EnableUpdate()
	{
		if(fUpdateEnabled)
			return;

		WorldManager::GetInstance().AddToUpdateList(*this);
		fUpdateEnabled = true;
	}

	void EntityIO::DisableFixedUpdate()
	{
		if(!fFixedUpdateEnabled)
			return;

		WorldManager::GetInstance().RemoveFromFixedUpdateList(*this);
		fFixedUpdateEnabled = false;
	}

	void EntityIO::DisableUpdate()
	{
		if(!fUpdateEnabled)
			return;

		WorldManager::GetInstance().RemoveFromUpdateList(*this);
		fUpdateEnabled = false;
	}

	EntityOutputManager::EntityOutputManager()
	{
		//empty
	}

	void EntityOutputManager::AddConnector(const String_t &name, OutputProcConnector_t proc)
	{
		ConnectorsMap_t::iterator it = mapConnectors.lower_bound(name);
		if((it != mapConnectors.end()) && (!mapConnectors.key_comp()(name, it->first)))
		{
			std::stringstream stream;
			stream << "Output " << name << " already exists.";
			PH_RAISE(OBJECT_ALREADY_EXISTS_EXCEPTION, "[EntityOutputManager::AddConnector]", stream.str());
		}

		mapConnectors.insert(it, std::make_pair(name, proc));		
	}

	void EntityOutputManager::Connect(EntityIO &outputOwner, const std::string &outputName, EntityIO &inputOwner, InputProc_t input)
	{
		ConnectorsMap_t::iterator it = mapConnectors.find(outputName);
		if(it == mapConnectors.end())
		{
			std::stringstream stream;
			stream << "Output " << outputName << " not found.";
			PH_RAISE(OBJECT_NOT_FOUND_EXCEPTION, "[EntityOutputManager::Connect]", stream.str());
		}

		(outputOwner.*(it->second))(inputOwner, input);
	}

	EntityInputManager::EntityInputManager()
	{
		//empty
	}

	void EntityInputManager::AddSlot(const String_t &name, InputProc_t proc)
	{
		InputMap_t::iterator it = mapInputs.lower_bound(name);
		if((it != mapInputs.end()) && (!mapInputs.key_comp()(name, it->first)))
		{
			std::stringstream stream;
			stream << "Output " << name << " already exists.";
			PH_RAISE(OBJECT_ALREADY_EXISTS_EXCEPTION, "[EntityInputManager::AddSlot]", stream.str());
		}

		mapInputs.insert(it, std::make_pair(name, proc));	
	}
}
