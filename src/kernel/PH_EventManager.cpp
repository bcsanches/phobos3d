/*
Phobos 3d
  January 2010

  Copyright (C) 2005-2010 Bruno Crivelari Sanches

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Bruno Crivelari Sanches bcsanches@gmail.com
*/

#include "PH_EventManager.h"

#include <boost/foreach.hpp>

#include "PH_Error.h"
#include "PH_Exception.h"
#include "PH_Folders.h"
#include "PH_Kernel.h"
#include "PH_Path.h"

namespace Phobos
{
	EventManagerPtr_t EventManager_c::ipInstance_gl;

	const String_c EventManager_c::DEFAULT_NAME("eventManager");

	EventManagerPtr_t EventManager_c::CreateInstance(const String_c &name)
	{
		PH_ASSERT_MSG(!ipInstance_gl, "[EventManager_c::CreateInstance]: Instance already exists");

		ipInstance_gl = EventManager_c::CreateInstanceImpl(name);

		Kernel_c::GetInstance().AddObject(ipInstance_gl, Path_c(PH_SYSTEM_FOLDER));

		return ipInstance_gl;
	}

	EventManagerPtr_t EventManager_c::GetInstance()
	{
		PH_ASSERT_MSG(ipInstance_gl, "[EventManager_c::GetInstance]: Instance does not exists, use CreateInstance");

		return ipInstance_gl;
	}

	void EventManager_c::ReleaseInstance()
	{	
		PH_ASSERT_MSG(ipInstance_gl, "[EventManager_c::ReleaseInstance]: Instance does not exists, use CreateInstance");

		ipInstance_gl->RemoveSelf();
		ipInstance_gl.reset();
	}

	const String_c &EventManager_c::GetDefaultName()
	{
		return DEFAULT_NAME;
	}

	EventManager_c::EventManager_c(const String_c &name):
		Node_c(name)
	{

	}

	void EventManager_c::AddListener(EventListener_c &listener, EventType_e type)
	{
		PH_ASSERT_MSG(type != EVENT_TYPE_NUM, "[EventManager_c::AddListener]: EventType value \"EVENT_TYPE_NUM\" is not valid for adding listeners");		

		arlstListeners[type].push_back(listener);
	}

	void EventManager_c::RemoveListener(EventListener_c &listener)
	{
		listener.hkListener.unlink();
	}

	bool EventManager_c::IsListenersListEmpty(EventType_e type)
	{
		PH_ASSERT(type < EVENT_TYPE_NUM);

		return arlstListeners[type].empty();
	}

	void EventManager_c::NotityListeners(Event_s &event)
	{
		PH_ASSERT(event.eType < EVENT_TYPE_NUM);

		BOOST_FOREACH(EventListener_c &listener, arlstListeners[event.eType])
		{
			listener.Event(event);
		}
	}
}
