/*
Phobos 3d
  March 2011

  Copyright (C) 2005-2011 Bruno Crivelari Sanches

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

#include <PH_Context.h>
#include <PH_ContextVar.h>
#include <PH_ContextUtils.h>
#include <PH_EventManager.h>
#include <PH_InputActions.h>
#include <PH_InputEvent.h>
#include <PH_InputManager.h>
#include <PH_InputMapper.h>
#include <PH_Kernel.h>
#include <PH_ProcVector.h>
#include <PH_Window.h>

using namespace Phobos;

class Sample_c: EventListener_c
{
	public:
		Sample_c();
		~Sample_c();

		void Run();

		void Event(struct Event_s &event);

	private:
		ProcVector_c		clSingletons;
		WindowPtr_t			ipWindow;
		EventManagerPtr_t	ipEventManager;
		InputManagerPtr_t	ipInputManager;
		InputMapperPtr_t	ipInputMapper;

		Context_c			clMainContext;

		ContextVar_c		varQuit;
};

Sample_c::Sample_c():
	varQuit("dvQuit", "false")
{
	Kernel_c::CreateInstance("Sample_04.log");
	clSingletons.AddProc(&Kernel_c::ReleaseInstance);

	ipWindow = Window_c::Create("RenderWindow");

	Rect_s<UInt_t> r(0, 0, 640, 480);
	ipWindow->Open("Sample 04", r);

	ipEventManager = EventManager_c::CreateInstance("EventManager");
	clSingletons.AddProc(&EventManager_c::ReleaseInstance);
	ipWindow->SetEventManager(ipEventManager);

	ipEventManager->AddListener(*this, EVENT_TYPE_SYSTEM);

	ipInputManager = InputManager_c::CreateInstance("InputManager");
	clSingletons.AddProc(&InputManager_c::ReleaseInstance);

	clMainContext.AddContextVar(varQuit);

	ipInputMapper = InputMapper_c::Create("InputMapper", clMainContext);

	//Force an update to allow device attachment
	ipInputManager->Update();

	ipInputMapper->Bind("kb", "ESCAPE", "set dvQuit true");
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
