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

#include <PH_Error.h>
#include <PH_Exception.h>
#include <PH_Folders.h>
#include <PH_Kernel.h>
#include <PH_Path.h>


Phobos::System::EventManagerPtr_t Phobos::System::EventManager_c::ipInstance_gl;

const Phobos::String_c Phobos::System::EventManager_c::DEFAULT_NAME("EventManager");

Phobos::System::EventManager_c &Phobos::System::EventManager_c::CreateInstance(const String_c &name)
{
	PH_ASSERT_MSG(!ipInstance_gl, "[EventManager_c::CreateInstance]: Instance already exists");

	ipInstance_gl = EventManager_c::CreateInstanceImpl(name);

	Kernel_c::GetInstance().AddObject(*ipInstance_gl, Path_c(PH_SYSTEM_FOLDER));

	return *ipInstance_gl;
}

Phobos::System::EventManager_c &Phobos::System::EventManager_c::GetInstance()
{
	PH_ASSERT_MSG(ipInstance_gl, "[EventManager_c::GetInstance]: Instance does not exists, use CreateInstance");

	return *ipInstance_gl;
}

void Phobos::System::EventManager_c::ReleaseInstance()
{	
	PH_ASSERT_MSG(ipInstance_gl, "[EventManager_c::ReleaseInstance]: Instance does not exists, use CreateInstance");
		
	ipInstance_gl.reset();
}

const Phobos::String_c &Phobos::System::EventManager_c::GetDefaultName()
{
	return DEFAULT_NAME;
}

Phobos::System::EventManager_c::EventManager_c(const String_c &name):
	Node_c(name)
{

}

void Phobos::System::EventManager_c::AddListener(Phobos::System::EventListener_c &listener, EventType_e type)
{
	PH_ASSERT_MSG(type != EVENT_TYPE_NUM, "[EventManager_c::AddListener]: EventType value \"EVENT_TYPE_NUM\" is not valid for adding listeners");		

	arlstListeners[type].push_back(listener);
}

void Phobos::System::EventManager_c::RemoveListener(EventListener_c &listener)
{
	listener.hkListener.unlink();
}

bool Phobos::System::EventManager_c::IsListenersListEmpty(EventType_e type)
{
	PH_ASSERT(type < EVENT_TYPE_NUM);

	return arlstListeners[type].empty();
}

void Phobos::System::EventManager_c::NotityListeners(Event_s &event)
{
	PH_ASSERT(event.eType < EVENT_TYPE_NUM);

	for(EventListener_c &listener : arlstListeners[event.eType])
	{				
		listener.Event(event);
	}
}

