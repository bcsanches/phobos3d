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

Phobos::System::EventManagerPtr_t Phobos::System::EventManager::ipInstance_gl;

const Phobos::String_t Phobos::System::EventManager::DEFAULT_NAME("EventManager");

Phobos::System::EventManager &Phobos::System::EventManager::CreateInstance(const String_t &name)
{
	PH_ASSERT_MSG(!ipInstance_gl, "[EventManager::CreateInstance]: Instance already exists");

	ipInstance_gl = EventManager::CreateInstanceImpl(name);

	Phobos::ObjectManager::AddObject(*ipInstance_gl, Path(PH_SYSTEM_FOLDER));

	return *ipInstance_gl;
}

Phobos::System::EventManager &Phobos::System::EventManager::GetInstance()
{
	PH_ASSERT_MSG(ipInstance_gl, "[EventManager::GetInstance]: Instance does not exists, use CreateInstance");

	return *ipInstance_gl;
}

void Phobos::System::EventManager::ReleaseInstance()
{	
	PH_ASSERT_MSG(ipInstance_gl, "[EventManager::ReleaseInstance]: Instance does not exists, use CreateInstance");
		
	ipInstance_gl.reset();
}

const Phobos::String_t &Phobos::System::EventManager::GetDefaultName()
{
	return DEFAULT_NAME;
}

Phobos::System::EventManager::EventManager(const String_t &name):
	Node(name)
{

}

void Phobos::System::EventManager::AddListener(Phobos::System::EventListener &listener, EventType_e type)
{
	PH_ASSERT_MSG(type != EVENT_TYPE_NUM, "[EventManager::AddListener]: EventType value \"EVENT_TYPE_NUM\" is not valid for adding listeners");		

	m_arlstListeners[type].push_back(listener);
}

void Phobos::System::EventManager::RemoveListener(EventListener &listener)
{
	listener.m_hkListener.unlink();
}

bool Phobos::System::EventManager::IsListenersListEmpty(EventType_e type)
{
	PH_ASSERT(type < EVENT_TYPE_NUM);

	return m_arlstListeners[type].empty();
}

void Phobos::System::EventManager::NotityListeners(Event_s &event)
{
	PH_ASSERT(event.m_eType < EVENT_TYPE_NUM);

	for(EventListener &listener : m_arlstListeners[event.m_eType])
	{				
		listener.OnEvent(event);
	}
}

