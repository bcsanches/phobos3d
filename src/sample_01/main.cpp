/*
Phobos 3d
February 2013
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


#include <Phobos/Memory.h>
#include <Phobos/ProcVector.h>

#include <Phobos/System/Window.h>
#include <Phobos/System/EventManager.h>

using namespace Phobos;

class Sample: Phobos::System::EventListener
{
	public:
		Sample();
		~Sample();

		void Run();

		void OnEvent(System::Event_s &event) override;

	private:
		ProcVector						m_clSingletons;
		Phobos::System::WindowPtr_t		m_ipWindow;
		Phobos::System::EventManager	*m_pclEventManager;

		bool m_fQuit;
};

Sample::Sample():
	m_fQuit(false)
{	
	m_ipWindow = Phobos::System::Window::Create("RenderWindow");
	
	m_ipWindow->Open("Sample 01", UIntSize_t(640, 480));

	m_pclEventManager = &Phobos::System::EventManager::CreateInstance("EventManager");
	m_clSingletons.AddProc(&Phobos::System::EventManager::ReleaseInstance);	

	m_pclEventManager->AddListener(*this, Phobos::System::EVENT_TYPE_SYSTEM);
}

Sample::~Sample()
{
	m_ipWindow.reset();

	m_clSingletons.CallAll();
}

void Sample::OnEvent(System::Event_s &event)
{
	switch(event.m_eType)
	{
		case Phobos::System::EVENT_TYPE_SYSTEM:
			if(event.m_stSystem.m_eType == Phobos::System::SYSTEM_QUIT)
			{
				m_fQuit = true;
				break;
			}
			break;

        default:
            break;
	}
}

void Sample::Run()
{
	while(!m_fQuit)
	{
		m_pclEventManager->Update();
	}
}

int main(int, char **)
{
	//Phobos::EnableMemoryTracker();
	//Phobos::BreakMemoryAllocation(150);

	{
		Sample sample;

		sample.Run();			
	}
	
	//Phobos::DumpMemoryLeaks();

	return 0;
}
