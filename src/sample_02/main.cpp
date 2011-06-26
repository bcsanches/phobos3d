/*
Phobos 3d
February 2011
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

#include <PH_EventManager.h>
#include <PH_InputActions.h>
#include <PH_InputEvent.h>
#include <PH_InputManager.h>
#include <PH_Kernel.h>
#include <PH_ProcVector.h>
#include <PH_Window.h>

using namespace Phobos;

class Sample_c: EventListener_c, InputManagerListener_c, InputDeviceListener_c
{
	public:
		Sample_c();
		~Sample_c();

		void Run();

		void Event(struct Event_s &event);
		void InputManagerEvent(const InputManagerEvent_s &event);

		void InputEvent(const InputEvent_s &event);

	private:
		ProcVector_c	clSingletons;
		WindowPtr_t		ipWindow;
		EventManagerPtr_t ipEventManager;
		InputManagerPtr_t ipInputManager;

		bool fQuit;
};

Sample_c::Sample_c():
	fQuit(false)
{
	Kernel_c::CreateInstance("Sample_02.log");
	clSingletons.AddProc(&Kernel_c::ReleaseInstance);

	ipWindow = Window_c::Create("RenderWindow");

	Rect_s<UInt_t> r(0, 0, 640, 480);
	ipWindow->Open("Sample 02", r);

	ipEventManager = EventManager_c::CreateInstance("EventManager");
	clSingletons.AddProc(&EventManager_c::ReleaseInstance);
	ipWindow->SetEventManager(ipEventManager);

	ipEventManager->AddListener(*this, EVENT_TYPE_SYSTEM);

	ipInputManager = InputManager_c::CreateInstance("InputManager");
	clSingletons.AddProc(&InputManager_c::ReleaseInstance);
	ipInputManager->AddListener(*this);
}

Sample_c::~Sample_c()
{
	ipWindow.reset();

	clSingletons.CallAll();
}

void Sample_c::Event(struct Event_s &event)
{
	switch(event.eType)
	{
		case EVENT_TYPE_SYSTEM:
			if(event.stSystem.eType == SYSTEM_QUIT)
			{
				fQuit = true;
				break;
			}
			break;

        default:
            break;
	}
}

void Sample_c::InputManagerEvent(const InputManagerEvent_s &event)
{
	switch(event.eType)
	{
		case INPUT_MANAGER_EVENT_DEVICE_ATTACHED:
			if(event.ipDevice->GetDeviceType() == INPUT_DEVICE_KEYBOARD)
				event.ipDevice->AddListener(*this);
			break;

        default:
            break;
	}
}

void Sample_c::InputEvent(const InputEvent_s &event)
{
	switch(event.eType)
	{
		case INPUT_EVENT_BUTTON:
			if((event.stButton.eState == BUTTON_STATE_DOWN) && (event.stButton.uId == KB_ESCAPE))
				fQuit = true;

        default:
            break;
	}
}

void Sample_c::Run()
{
	while(!fQuit)
	{
		ipEventManager->Update();
		ipInputManager->Update();
	}
}

int main(int, char **)
{
	Sample_c sample;

	sample.Run();

	return 0;
}
