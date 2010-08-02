#ifndef PH_BASE_LISTENER_H
#define PH_BASE_LISTENER_H

#include <boost/intrusive/list.hpp>

#define PH_DECLARE_LISTENER_HOOK																							\
	typedef boost::intrusive::list_member_hook<boost::intrusive::link_mode<boost::intrusive::auto_unlink> > ListenerHook_t;	\
	ListenerHook_t hkListener;
	
#define PH_DECLARE_LISTENER_LIST_TYPE(X)\
	typedef boost::intrusive::member_hook<X, X::ListenerHook_t, &X::hkListener> ListenerMemberHookOption_t;					\
	typedef boost::intrusive::list<X, boost::intrusive::constant_time_size<false>, ListenerMemberHookOption_t > ListenersList_t;

#endif