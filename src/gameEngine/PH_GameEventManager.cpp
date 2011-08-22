/*
Phobos 3d
July 2011
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

#include "PH_GameEventManager.h"

#include <PH_Core.h>

namespace Phobos
{
	PH_DEFINE_DEFAULT_SINGLETON(GameEventManager);	

	GameEventManager_c::GameEventManager_c():
		CoreModule_c("GameEventManager", PRIVATE_CHILDREN)
	{
		//empty
	}

	GameEventManager_c::~GameEventManager_c()
	{
	}

	void GameEventManager_c::OnFixedUpdate()
	{
		Float_t ticks = Core_c::GetInstance()->GetGameTimer().fpTotalTicks;

		while(!mapEvents.empty())
		{
			GameEventMap_t::iterator it = mapEvents.begin();
			
			if(it->first > ticks)
				break;

			GameEvent_c event(it->second);
			mapEvents.erase(it);

			event.Fire();
		}
	}

	void GameEventManager_c::Schedule(EntityIO_c &receiver, GameEvent_c::EventProc_t proc, Float_t delay)
	{	
		PH_ASSERT_VALID(proc);

		mapEvents.insert(std::make_pair(delay + Core_c::GetInstance()->GetGameTimer().fpTotalTicks, GameEvent_c(receiver, proc)));		
	}

	void GameEventManager_c::CancelEvents(const EntityIO_c &receiver)
	{
		for(GameEventMap_t::iterator it = mapEvents.begin();it != mapEvents.end();)
		{
			GameEventMap_t::iterator eraseIt = it++;

			if(&eraseIt->second.GetReceiver() == &receiver)
			{
				mapEvents.erase(eraseIt);
			}
		}
	}
}

