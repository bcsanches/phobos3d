/*
Phobos 3d
February 2011
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

#include <Phobos/Log.h>
#include <Phobos/Memory.h>
#include <Phobos/ProcVector.h>

#include <Phobos/System/Window.h>
#include <Phobos/System/EventManager.h>
#include <Phobos/System/InputActions.h>
#include <Phobos/System/InputDevice.h>
#include <Phobos/System/InputEvent.h>
#include <Phobos/System/InputManager.h>
#include <Phobos/System/Window.h>

using namespace Phobos;

class Sample: System::EventListener, System::InputManagerListener, System::InputDeviceListener
{
	public:
		Sample();
		~Sample();

		void Run();

		virtual void OnEvent(System::Event_s &event) override;
		virtual void OnInputManagerEvent(const System::InputManagerEvent_s &event) override;

		virtual void OnInputEvent(const System::InputEvent_s &event) override;

	private:
		ProcVector					m_clSingletons;
		System::WindowPtr_t			m_ipWindow;		
		System::InputManagerPtr_t	m_ipInputManager;

		bool m_fQuit;
};

Sample::Sample():
	m_fQuit(false)
{	
	Phobos::LogChangeFile("Sample02.log");

	m_ipWindow = System::Window::Create("RenderWindow");
	
	m_ipWindow->Open("Sample 02", UIntSize_t(640, 480));

	auto &eventManager = System::EventManager::CreateInstance("EventManager");
	m_clSingletons.AddProc(&System::EventManager::ReleaseInstance);	

	eventManager.AddListener(*this, System::EVENT_TYPE_SYSTEM);

	m_ipInputManager = System::InputManager::Create("InputManager");
	m_ipInputManager->AddListener(*this);

	m_ipInputManager->GetDevice(System::INPUT_DEVICE_KEYBOARD).AddListener(*this);
}

Sample::~Sample()
{
	m_ipInputManager.reset();

	m_ipWindow.reset();

	m_clSingletons.CallAll();
}

void Sample::OnEvent(System::Event_s &event)
{
	switch(event.m_eType)
	{
		case System::EVENT_TYPE_SYSTEM:
			if(event.m_stSystem.m_eType == System::SYSTEM_QUIT)
			{
				m_fQuit = true;
				break;
			}
			break;

        default:
            break;
	}
}

void Sample::OnInputManagerEvent(const System::InputManagerEvent_s &event)
{
	switch(event.m_eType)
	{
		case System::INPUT_MANAGER_EVENT_DEVICE_ATTACHED:
			if(event.m_rclDevice.GetDeviceType() == System::INPUT_DEVICE_KEYBOARD)
				event.m_rclDevice.AddListener(*this);
			break;

        default:
            break;
	}
}

void Sample::OnInputEvent(const System::InputEvent_s &event)
{
	switch(event.m_eType)
	{
		case System::INPUT_EVENT_BUTTON:
			if((event.m_stButton.m_eState == System::BUTTON_STATE_DOWN) && (event.m_stButton.m_uId == System::KB_ESCAPE))
				m_fQuit = true;

        default:
            break;
	}
}

void Sample::Run()
{
	while(!m_fQuit)
	{
		System::EventManager::GetInstance().Update();
		m_ipInputManager->Update();
	}
}

int main(int, char **)
{
	Phobos::EnableMemoryTracker();
	Phobos::BreakMemoryAllocation(150);

	{
		Sample sample;

		sample.Run();
	}

	Phobos::DumpMemoryLeaks();

	return 0;
}
