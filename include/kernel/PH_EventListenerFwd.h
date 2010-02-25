#ifndef PH_EVENT_LISTENER_FWD_H
#define PH_EVENT_LISTENER_FWD_H

#include <boost/intrusive/list.hpp>

namespace Phobos
{
	class EventListener_c;

	typedef boost::intrusive::list_member_hook<boost::intrusive::link_mode<boost::intrusive::auto_unlink> > EventListenerListHook_t;	
}

#endif
