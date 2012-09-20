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

#include "PH_EventManagerModule.h"

#include <PH_Kernel.h>
#include <PH_EventManager.h>
#include <PH_Memory.h>

#include "PH_Console.h"
#include "PH_Core.h"

namespace Phobos
{

	EventManagerModulePtr_t EventManagerModule_c::ipInstance_gl;

	EventManagerModule_c::EventManagerModule_c():
		CoreModule_c("EventManagerModule")
	{
		EventManager_c &manager = EventManager_c::CreateInstance("EventManager");
		manager.AddListener(*this, EVENT_TYPE_SYSTEM);		
	}

	EventManagerModule_c::~EventManagerModule_c()
	{
		EventManager_c::ReleaseInstance();
	}

	EventManagerModule_c &EventManagerModule_c::CreateInstance()
	{
		PH_ASSERT_MSG(!ipInstance_gl, "[EventManagerModule_c::CreateInstance]: Instance already exists");

		ipInstance_gl = EventManagerModulePtr_t(PH_NEW EventManagerModule_c());

		return *ipInstance_gl;
	}

	void EventManagerModule_c::ReleaseInstance()
	{
		PH_ASSERT_MSG(ipInstance_gl, "[EventManagerModule_c::ReleaseInstance]: Instance does not exists, use CreateInstance");

		ipInstance_gl.reset();
	}

	EventManagerModule_c &EventManagerModule_c::GetInstance()
	{
		PH_ASSERT_MSG(ipInstance_gl, "[EventManagerModule_c::GetInstance]: Instance does not exists, use CreateInstance");

		return *ipInstance_gl;
	}

	void EventManagerModule_c::OnFixedUpdate()
	{
		EventManager_c::GetInstance().Update();
	}

	void EventManagerModule_c::Event(struct Event_s &event)
	{
		switch(event.stSystem.eType)
		{
			case SYSTEM_QUIT:
				Console_c::GetInstance().Execute("quit");
				break;

            default:
                break;
		}
	}
}
