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

#include "Phobos/Game/Things/SignalManager.h"

#include <Phobos/Engine/Core.h>

namespace Phobos
{
	namespace Game
	{
		namespace Things
		{
			PH_DEFINE_DEFAULT_SINGLETON(SignalManager);	

			SignalManager::SignalManager():
				Module("GameEventManager", NodeFlags::PRIVATE_CHILDREN)
			{
				//empty
			}

			SignalManager::~SignalManager()
			{
			}

			void SignalManager::OnFixedUpdate()
			{
				Float_t ticks = Engine::Core::GetInstance().GetGameTimer().m_fpTotalTicks;

				while(!m_mmapSignals.empty())
				{
					auto it = m_mmapSignals.begin();
			
					if(it->first > ticks)
						break;

					Signal event(it->second);
					m_mmapSignals.erase(it);

					event.Fire();
				}
			}

			void SignalManager::Schedule(Thing &receiver, Signal::SignalProc_t proc, Float_t delay)
			{	
				PH_ASSERT_VALID(proc);

				m_mmapSignals.insert(std::make_pair(delay + Engine::Core::GetInstance().GetGameTimer().m_fpTotalTicks, Signal(receiver, proc)));		
			}

			void SignalManager::CancelEvents(const Thing &receiver)
			{
				for(auto it = m_mmapSignals.begin();it != m_mmapSignals.end();)
				{
					auto eraseIt = it++;

					if(&eraseIt->second.GetReceiver() == &receiver)
					{
						m_mmapSignals.erase(eraseIt);
					}
				}
			}
		}
	}
}

