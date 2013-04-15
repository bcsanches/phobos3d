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

#include <Phobos/Engine/Core.h>

namespace Phobos
{
	PH_DEFINE_DEFAULT_SINGLETON(GameEventManager);	

	GameEventManager::GameEventManager():
		Module("GameEventManager", NodeFlags::PRIVATE_CHILDREN)
	{
		//empty
	}

	GameEventManager::~GameEventManager()
	{
	}

	void GameEventManager::OnFixedUpdate()
	{
		Float_t ticks = Engine::Core::GetInstance().GetGameTimer().m_fpTotalTicks;

		while(!m_mapEvents.empty())
		{
			GameEventMap_t::iterator it = m_mapEvents.begin();
			
			if(it->first > ticks)
				break;

			GameEvent event(it->second);
			m_mapEvents.erase(it);

			event.Fire();
		}
	}

	void GameEventManager::Schedule(EntityIO &receiver, GameEvent::EventProc_t proc, Float_t delay)
	{	
		PH_ASSERT_VALID(proc);

		m_mapEvents.insert(std::make_pair(delay + Engine::Core::GetInstance().GetGameTimer().m_fpTotalTicks, GameEvent(receiver, proc)));		
	}

	void GameEventManager::CancelEvents(const EntityIO &receiver)
	{
		for(GameEventMap_t::iterator it = m_mapEvents.begin();it != m_mapEvents.end();)
		{
			GameEventMap_t::iterator eraseIt = it++;

			if(&eraseIt->second.GetReceiver() == &receiver)
			{
				m_mapEvents.erase(eraseIt);
			}
		}
	}
}

