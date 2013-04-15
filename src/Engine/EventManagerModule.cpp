/*
Phobos 3d
January 2010
Copyright (c) 2005-2013 Bruno Sanches  http://code.google.com/p/phobos3d

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#include "Phobos/Engine/EventManagerModule.h"

#include <Phobos/Memory.h>

#include "Phobos/Engine/Console.h"
#include "Phobos/Engine/Core.h"

Phobos::Engine::EventManagerModulePtr_t Phobos::Engine::EventManagerModule::ipInstance_gl;

Phobos::Engine::EventManagerModule::EventManagerModule():
	Module("EventManagerModule")
{
	auto &manager = System::EventManager::CreateInstance("EventManager");
	manager.AddListener(*this, System::EVENT_TYPE_SYSTEM);		
}

Phobos::Engine::EventManagerModule::~EventManagerModule()
{
	System::EventManager::ReleaseInstance();
}

Phobos::Engine::EventManagerModule &Phobos::Engine::EventManagerModule::CreateInstance()
{
	PH_ASSERT_MSG(!ipInstance_gl, "[EventManagerModule::CreateInstance]: Instance already exists");

	ipInstance_gl = EventManagerModulePtr_t(PH_NEW EventManagerModule());

	return *ipInstance_gl;
}

void Phobos::Engine::EventManagerModule::ReleaseInstance()
{
	PH_ASSERT_MSG(ipInstance_gl, "[EventManagerModule::ReleaseInstance]: Instance does not exists, use CreateInstance");

	ipInstance_gl.reset();
}

Phobos::Engine::EventManagerModule &Phobos::Engine::EventManagerModule::GetInstance()
{
	PH_ASSERT_MSG(ipInstance_gl, "[EventManagerModule::GetInstance]: Instance does not exists, use CreateInstance");

	return *ipInstance_gl;
}

void Phobos::Engine::EventManagerModule::OnFixedUpdate()
{
	System::EventManager::GetInstance().Update();
}

void Phobos::Engine::EventManagerModule::OnEvent(System::Event_s &event)
{
	switch(event.m_stSystem.m_eType)
	{
		case System::SYSTEM_QUIT:
			Console::GetInstance().Execute("quit");
			break;

        default:
            break;
	}
}
