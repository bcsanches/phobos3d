/*
Phobos 3d
March 2011
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

#include <Phobos/Shell/Context.h>
#include <Phobos/Shell/Utils.h>

#include <Phobos/System/EventManager.h>
#include <Phobos/System/InputActions.h>
#include <Phobos/System/InputEvent.h>
#include <Phobos/System/InputManager.h>
#include <Phobos/System/InputMapper.h>
#include <Phobos/System/Window.h>

using namespace Phobos;

class Sample: System::EventListener
{
	public:
		Sample();
		~Sample();

		void Run();

		void OnEvent(System::Event_s &event) override;

	private:
		void CmdQuit(const Shell::StringVector_t &args, Shell::Context &);

	private:
		ProcVector					m_clSingletons;
		System::WindowPtr_t			m_ipWindow;		
		System::InputManagerPtr_t	m_ipInputManager;
		System::InputMapperPtr_t	m_ipInputMapper;

		Shell::Context				m_clMainContext;

		Shell::Command				m_cmdQuit;

		bool						m_fQuit;
};

Sample::Sample():
	m_cmdQuit("quit"),
	m_fQuit(false)
{
	LogChangeFile("Sample_03.log");
	
	m_ipWindow = System::Window::Create("RenderWindow");
	
	m_ipWindow->Open("Sample 03", UIntSize_t(640, 480));

	auto &eventManager = System::EventManager::CreateInstance("EventManager");
	m_clSingletons.AddProc(&System::EventManager::ReleaseInstance);	

	eventManager.AddListener(*this, System::EVENT_TYPE_SYSTEM);

	m_ipInputManager = System::InputManager::Create("InputManager");	

	m_cmdQuit.SetProc(PH_CONTEXT_CMD_BIND(&Sample::CmdQuit, this));
	m_clMainContext.AddContextCommand(m_cmdQuit);

	m_ipInputMapper = System::InputMapper::Create("InputMapper", m_clMainContext, *m_ipInputManager);
	
	m_ipInputMapper->Bind("kb", "ESCAPE", "quit");
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

//Called automatically by the input mapper due to input mapping
void Sample::CmdQuit(const Shell::StringVector_t &, Shell::Context &)
{
	m_fQuit = true;
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

	{
		Sample sample;

		sample.Run();
	}

	Phobos::DumpMemoryLeaks();

	return 0;
}
