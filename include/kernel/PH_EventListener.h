#ifndef PH_EVENT_LISTENER_H
#define PH_EVENT_LISTENER_H

#include "PH_EventListenerFwd.h"

namespace Phobos
{
	class EventListener_c
	{					
		public:
			virtual ~EventListener_c() {};

			virtual void Event(struct Event_s &event) = 0;
								
			EventListenerListHook_t hkEventManager;
	};
}

#endif