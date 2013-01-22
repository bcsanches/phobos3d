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



#include <PH_Kernel.h>
#include <PH_Memory.h>
#include <PH_ProcVector.h>

#include <Phobos/System/Window.h>
#include <Phobos/System/EventManager.h>
#include <Phobos/System/InputActions.h>
#include <Phobos/System/InputDevice.h>
#include <Phobos/System/InputEvent.h>
#include <Phobos/System/InputManager.h>
#include <Phobos/System/Window.h>

using namespace Phobos;

class Sample_c: System::EventListener_c, System::InputManagerListener_c, System::InputDeviceListener_c
{
	public:
		Sample_c();
		~Sample_c();

		void Run();

		void Event(System::Event_s &event);
		void InputManagerEvent(const System::InputManagerEvent_s &event);

		void InputEvent(const System::InputEvent_s &event);

	private:
		ProcVector_c	clSingletons;
		System::WindowPtr_t		ipWindow;				

		bool fQuit;
};

Sample_c::Sample_c():
	fQuit(false)
{
	Kernel_c::CreateInstance("Sample_02.log");
	clSingletons.AddProc(&Kernel_c::ReleaseInstance);

	ipWindow = System::Window_c::Create("RenderWindow");

	Rect_s<UInt_t> r(0, 0, 640, 480);
	ipWindow->Open("Sample 02", r);

	auto &eventManager = System::EventManager_c::CreateInstance("EventManager");
	clSingletons.AddProc(&System::EventManager_c::ReleaseInstance);	

	eventManager.AddListener(*this, System::EVENT_TYPE_SYSTEM);

	auto &inputManager = System::InputManager_c::CreateInstance("InputManager");
	clSingletons.AddProc(&System::InputManager_c::ReleaseInstance);
	inputManager.AddListener(*this);
}

Sample_c::~Sample_c()
{
	ipWindow.reset();

	clSingletons.CallAll();
}

void Sample_c::Event(System::Event_s &event)
{
	switch(event.eType)
	{
		case System::EVENT_TYPE_SYSTEM:
			if(event.stSystem.eType == System::SYSTEM_QUIT)
			{
				fQuit = true;
				break;
			}
			break;

        default:
            break;
	}
}

void Sample_c::InputManagerEvent(const System::InputManagerEvent_s &event)
{
	switch(event.eType)
	{
		case System::INPUT_MANAGER_EVENT_DEVICE_ATTACHED:
			if(event.rclDevice.GetDeviceType() == System::INPUT_DEVICE_KEYBOARD)
				event.rclDevice.AddListener(*this);
			break;

        default:
            break;
	}
}

void Sample_c::InputEvent(const System::InputEvent_s &event)
{
	switch(event.eType)
	{
		case System::INPUT_EVENT_BUTTON:
			if((event.stButton.eState == System::BUTTON_STATE_DOWN) && (event.stButton.uId == System::KB_ESCAPE))
				fQuit = true;

        default:
            break;
	}
}

void Sample_c::Run()
{
	while(!fQuit)
	{
		System::EventManager_c::GetInstance().Update();
		System::InputManager_c::GetInstance().Update();		
	}
}

int main(int, char **)
{
	Phobos::EnableMemoryTracker();
	Phobos::BreakMemoryAllocation(150);

	{
		Sample_c sample;

		sample.Run();
	}

	Phobos::DumpMemoryLeaks();

	return 0;
}
