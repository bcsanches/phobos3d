/*
Phobos 3d
January 2010
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

#include "Phobos/System/EventManager.h"

#include <Phobos/Error.h>
#include <Phobos/Exception.h>
#include <Phobos/Folders.h>
#include <Phobos/ObjectManager.h>
#include <Phobos/Path.h>

namespace Phobos
{
	namespace System
	{
		namespace EventManager
		{
			PH_DECLARE_LISTENER_LIST_TYPE(EventListener);

			static ListenersList_t g_arlstListeners[EVENT_TYPE_NUM];

			void AddListener(Phobos::System::EventListener &listener, EventType_e type)
			{
				PH_ASSERT_MSG(type != EVENT_TYPE_NUM, "[EventManager::AddListener]: EventType value \"EVENT_TYPE_NUM\" is not valid for adding listeners");

				g_arlstListeners[type].push_back(listener);
			}

			void RemoveListener(EventListener &listener)
			{
				listener.m_hkListener.unlink();
			}

			bool IsListenersListEmpty(EventType_e type)
			{
				PH_ASSERT(type < EVENT_TYPE_NUM);

				return g_arlstListeners[type].empty();
			}

			void PumpEvents()
			{
				detail::DoPumpEvents();
			}

			namespace detail
			{
				void NotityListeners(Event_s &event)
				{
					PH_ASSERT(event.m_eType < EVENT_TYPE_NUM);

					for (EventListener &listener : g_arlstListeners[event.m_eType])
					{
						listener.OnEvent(event);
					}
				}
			}
		}
	}
}




