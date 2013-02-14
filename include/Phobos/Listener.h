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

#ifndef PH_SYSTEM_BASE_LISTENER_H
#define PH_SYSTEM_BASE_LISTENER_H

#include <boost/intrusive/list.hpp>

#define PH_DECLARE_LISTENER_HOOK																							\
	typedef boost::intrusive::list_member_hook<boost::intrusive::link_mode<boost::intrusive::auto_unlink> > ListenerHook_t;	\
	ListenerHook_t m_hkListener;
	
#define PH_DECLARE_LISTENER_LIST_TYPE(X)\
	typedef boost::intrusive::member_hook<X, X::ListenerHook_t, &X::m_hkListener> ListenerMemberHookOption_t;					\
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
		listener.m_hkListener.unlink();			\
	}


#endif