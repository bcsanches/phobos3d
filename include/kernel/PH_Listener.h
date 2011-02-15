#ifndef PH_BASE_LISTENER_H
#define PH_BASE_LISTENER_H

#include <boost/intrusive/list.hpp>

#define PH_DECLARE_LISTENER_HOOK																							\
	typedef boost::intrusive::list_member_hook<boost::intrusive::link_mode<boost::intrusive::auto_unlink> > ListenerHook_t;	\
	ListenerHook_t hkListener;
	
#define PH_DECLARE_LISTENER_LIST_TYPE(X)\
	typedef boost::intrusive::member_hook<X, X::ListenerHook_t, &X::hkListener> ListenerMemberHookOption_t;					\
	typedef boost::intrusive::list<X, boost::intrusive::constant_time_size<false>, ListenerMemberHookOption_t > ListenersList_t;

#define PH_DECLARE_LISTENER_LIST(X, NAME)	\
	PH_DECLARE_LISTENER_LIST_TYPE(X)		\
	ListenersList_t NAME;

#define PH_DECLARE_LISTENER_PROCS(X)	\
	void AddListener(X &listener);		\
	void RemoveListener(X &listener);

#define PH_DEFINE_LISTENER_PROCS(OWNER, X, LIST)\
	void OWNER::AddListener(X &listener)		\
	{											\
		LIST.push_back(listener);				\
	}											\
												\
	void OWNER::RemoveListener(X &listener)		\
	{											\
		listener.hkListener.unlink();			\
	}


#endif