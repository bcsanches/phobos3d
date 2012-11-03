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


#include <PH_Kernel.h>
#include <PH_Memory.h>
#include <PH_ProcVector.h>
#include <PH_Window.h>
#include <PH_EventManager.h>

using namespace Phobos;

class Sample_c: EventListener_c
{
	public:
		Sample_c();
		~Sample_c();

		void Run();

		void Event(struct Event_s &event);

	private:
		ProcVector_c	clSingletons;
		WindowPtr_t		ipWindow;
		EventManager_c	*pclEventManager;

		bool fQuit;
};

Sample_c::Sample_c():
	fQuit(false)
{
	Kernel_c::CreateInstance("Sample_01.log");
	clSingletons.AddProc(&Kernel_c::ReleaseInstance);

	ipWindow = Window_c::Create("RenderWindow");

	Rect_s<UInt_t> r(0, 0, 640, 480);
	ipWindow->Open("Sample 01", r);

	pclEventManager = &EventManager_c::CreateInstance("EventManager");
	clSingletons.AddProc(&EventManager_c::ReleaseInstance);	

	pclEventManager->AddListener(*this, EVENT_TYPE_SYSTEM);
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

void Sample_c::Run()
{
	while(!fQuit)
	{
		pclEventManager->Update();
	}
}

int main(int, char **)
{
	//Phobos::EnableMemoryTracker();
	//Phobos::BreakMemoryAllocation(150);

	{
		Sample_c sample;

		sample.Run();			
	}
	
	//Phobos::DumpMemoryLeaks();

	return 0;
}
