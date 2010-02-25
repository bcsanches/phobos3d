#ifndef PH_EVENT_MANAGER_H
#define PH_EVENT_MANAGER_H

#include <boost/intrusive_ptr.hpp>

namespace Phobos
{
	class EventManager_c;

	typedef boost::intrusive_ptr<EventManager_c> EventManagerPtr_t;
}

#endif