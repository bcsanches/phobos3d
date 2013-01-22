/*
Phobos 3d
March 2011
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

#include <PH_Context.h>
#include <PH_ContextVar.h>
#include <PH_ContextUtils.h>
#include <PH_Kernel.h>
#include <PH_Memory.h>
#include <PH_ProcVector.h>

#include <Phobos/System/EventManager.h>
#include <Phobos/System/InputActions.h>
#include <Phobos/System/InputEvent.h>
#include <Phobos/System/InputManager.h>
#include <Phobos/System/InputMapper.h>
#include <Phobos/System/Window.h>

using namespace Phobos;

class Sample_c: System::EventListener_c
{
	public:
		Sample_c();
		~Sample_c();

		void Run();

		void Event(System::Event_s &event);

	private:
		ProcVector_c		clSingletons;
		System::WindowPtr_t			ipWindow;				
		System::InputMapperPtr_t	ipInputMapper;

		Context_c			clMainContext;

		ContextVar_c		varQuit;
};

Sample_c::Sample_c():
	varQuit("dvQuit", "false")
{
	Kernel_c::CreateInstance("Sample_04.log");
	clSingletons.AddProc(&Kernel_c::ReleaseInstance);

	ipWindow = System::Window_c::Create("RenderWindow");

	Rect_s<UInt_t> r(0, 0, 640, 480);
	ipWindow->Open("Sample 04", r);

	auto &eventManager = System::EventManager_c::CreateInstance("EventManager");
	clSingletons.AddProc(&System::EventManager_c::ReleaseInstance);	

	eventManager.AddListener(*this, System::EVENT_TYPE_SYSTEM);

	auto &inputManager = System::InputManager_c::CreateInstance("InputManager");
	clSingletons.AddProc(&System::InputManager_c::ReleaseInstance);

	clMainContext.AddContextVar(varQuit);

	ipInputMapper = System::InputMapper_c::Create("InputMapper", clMainContext);

	//Force an update to allow device attachment
	inputManager.Update();

	ipInputMapper->Bind("kb", "ESCAPE", "set dvQuit true");
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
				varQuit.SetValue("true");
				break;
			}
			break;

        default:
            break;
	}
}

void Sample_c::Run()
{
	while(!varQuit.GetBoolean())
	{
		System::EventManager_c::GetInstance().Update();
		System::InputManager_c::GetInstance().Update();
	}
}

int main(int, char **)
{
	Phobos::EnableMemoryTracker();

	{
		Sample_c sample;

		sample.Run();
	}

	Phobos::DumpMemoryLeaks();

	return 0;
}
