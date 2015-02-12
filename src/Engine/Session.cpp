/*
Phobos 3d
October 2012
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

#include "Phobos/Engine/Session.h"

#include <Phobos/System/EventManager.h>
#include <Phobos/System/InputActions.h>
#include <Phobos/System/InputDevice.h>
#include <Phobos/System/InputEvent.h>
#include <Phobos/System/InputManager.h>
#include <Phobos/System/InputMapper.h>
#include <Phobos/System/MouseInputDevice.h>

#include "Phobos/Engine/Gui/Form.h"
#include "Phobos/Engine/Gui/Manager.h"

#include "Phobos/Engine/Console.h"
#include "Phobos/Engine/Client.h"
#include "Phobos/Engine/IPlayerCommandProducer.h"
#include "Phobos/Engine/ModuleFactory.h"

#define CONSOLE_KEY '`'

namespace
{
	static Phobos::Engine::Session *g_pclSession = nullptr;
}

PH_MODULE_FULL_CREATOR("Session", Phobos::Engine::Session);

Phobos::Engine::Session &Phobos::Engine::Session::GetInstance()
{
	PH_ASSERT_VALID(g_pclSession);

	return *g_pclSession;
}

Phobos::Engine::Session::Session(const String_t &name):
	Module(name, PRIVATE_CHILDREN),
	m_fIgnoreConsoleKey(false),
	m_pclPlayerCommandProducer(nullptr),
	m_pclClient(nullptr),
	m_pclForm(nullptr)
{
	PH_ASSERT(g_pclSession == nullptr);

	g_pclSession = this;

	System::EventManager::AddListener(*this, System::EVENT_TYPE_SYSTEM);

	m_ipInputManager = System::InputManager::Create("InputManager");

	this->AddPrivateChild(*m_ipInputManager);

	m_ipInputManager->GetDevice(Phobos::System::INPUT_DEVICE_KEYBOARD).AddListener(*this);

	m_ipInputMapper = std::make_shared<System::InputMapper>("InputMapper", Console::GetInstance(), *m_ipInputManager);
	m_ipInputMapper->Disable();

	this->AddPrivateChild(*m_ipInputMapper);
}

Phobos::Engine::Session::~Session()
{
	PH_ASSERT(g_pclSession == this);

	g_pclSession = nullptr;
}

void Phobos::Engine::Session::OnInputManagerEvent(const System::InputManagerEvent_s &event)
{
	std::stringstream stream;

	switch(event.m_eType)
	{
		case System::INPUT_MANAGER_EVENT_DEVICE_ATTACHED:
			stream << "[Console::InputManagerEvent] Device " << event.m_rclDevice.GetName() << " attached.";
			if(event.m_rclDevice.GetDeviceType() == System::INPUT_DEVICE_KEYBOARD)
				event.m_rclDevice.AddListener(*this);
			break;

		case System::INPUT_MANAGER_EVENT_DEVICE_DETACHED:
			stream << "[Console::InputManagerEvent] Device " << event.m_rclDevice.GetName() << " detached.";
			break;
	}

	LogMessage(stream.str());
}

void Phobos::Engine::Session::OnInputEvent(const System::InputEvent_s &event)
{
	auto &console = Console::GetInstance();

	if((event.m_eType == System::INPUT_EVENT_BUTTON) && (event.m_stButton.m_eState == Phobos::System::BUTTON_STATE_DOWN))
	{		
		if(event.m_stButton.m_uId == Phobos::System::KB_ESCAPE)
		{
			if(console.IsActive())
			{
				this->CloseConsole();				
			}
			else
			{
				//No console, so someone else must handle this

				Gui::Form *newForm = nullptr;
				EscAction action = EscAction::IGNORE_ESC;

				if(m_pclForm)
					action = m_pclForm->HandleEsc(newForm);
				else if(m_pclClient)
					action = m_pclClient->HandleEsc(newForm);

				if(action == EscAction::SET_GUI)
				{
					this->SetGuiForm(newForm);
				}

			}
		}
		else if((event.m_stButton.m_uId == CONSOLE_KEY || event.m_stButton.m_uId == '\'') && !console.IsActive())
		{
			m_fIgnoreConsoleKey = true;
			console.ToggleConsole();

			if(m_pclForm)
			{
				Gui::Manager::GetInstance().DisableInput(*m_ipInputManager);
				this->GetConfig().m_pclMouse->ShowCursor();
			}
			else
			{			
				this->DisableGameInput();
			}
		}
	}

	if(console.IsActive())
	{
		if(event.m_eType == System::INPUT_EVENT_CHAR)
		{			
			if(m_fIgnoreConsoleKey && (event.m_stButton.m_uId == CONSOLE_KEY || event.m_stButton.m_uId == '\''))
			{
				m_fIgnoreConsoleKey = false;
				return;
			}
			m_fIgnoreConsoleKey = false;
		}		

		console.HandleInputEvent(event);	
	}
}

void Phobos::Engine::Session::OnFixedUpdate()
{
	System::EventManager::PumpEvents();

	m_ipInputManager->Update();	

	auto &console = Console::GetInstance();

	console.FlushCommandBuffer();

	if(!console.IsActive())
	{
		if((m_pclPlayerCommandProducer) && (m_pclClient))
		{
			IPlayerCmdPtr_t cmd = m_pclPlayerCommandProducer->CreateCmd();
			m_pclClient->DispatchCommand(cmd);
		}
	}

	if (m_pclClient)
		m_pclClient->FixedUpdate();
}

void Phobos::Engine::Session::OnUpdate()
{
	if (m_pclClient)
		m_pclClient->Update();
}

void Phobos::Engine::Session::SetPlayerCommandProducer(IPlayerCommandProducer *commandProducer)
{	
	auto &console = Console::GetInstance();

	if(m_pclPlayerCommandProducer)
	{
		m_pclPlayerCommandProducer->Disable();		
	}

	if (!console.IsActive() && !m_pclForm)
		this->DisableGameInput();

	m_pclPlayerCommandProducer = commandProducer;
	
	if(m_pclPlayerCommandProducer)
	{
		if(m_pclClient)
		{
			//Sets a default cmd
			IPlayerCmdPtr_t cmd = m_pclPlayerCommandProducer->CreateCmd();
			m_pclClient->DispatchCommand(cmd);
		}				
	}
	else if(!console.IsActive())
	{
		this->UnclipMouseCursor();
	}

	if (!console.IsActive() && (!m_pclForm))
	{
		this->EnableGameInput();
	}
}

void Phobos::Engine::Session::SetClient(Client *client)
{
	if (m_pclClient)
		m_pclClient->Disconnect();

	m_pclClient = client;

	if (m_pclClient)
		m_pclClient->Connect();
}

void Phobos::Engine::Session::CloseConsole()
{
	Console &console = Console::GetInstance();
	if(!console.IsActive())
		return;

	//Close console
	console.ToggleConsole();

	//now give input focus to game or current form

	if(m_pclForm)
	{
		Gui::Manager::GetInstance().EnableInput(*m_ipInputManager);
		this->GetConfig().m_pclMouse->HideCursor();
	}
	else
	{
		this->EnableGameInput();
	}
}

void Phobos::Engine::Session::SetGuiForm(Gui::Form *newForm)
{
	Console &console = Console::GetInstance();

	if ((m_pclForm == nullptr) && (!console.IsActive()))
	{
		//no previous form, so game active, disable it
		this->DisableGameInput();
	}

	if(m_pclForm)
	{
		m_pclForm->Close();
	}

	m_pclForm = newForm;

	if(!console.IsActive())
	{
		if(m_pclForm != nullptr)
		{
			Gui::Manager::GetInstance().EnableInput(*m_ipInputManager);

			this->GetConfig().m_pclMouse->HideCursor();
		}
		else
		{
			Gui::Manager::GetInstance().DisableInput(*m_ipInputManager);
			this->GetConfig().m_pclMouse->ShowCursor();
			
			this->EnableGameInput();
		}
	}
}

Phobos::Engine::Session::ConfigInfo_s Phobos::Engine::Session::GetConfig()
{
	ConfigInfo_s info;
	
	info.m_pclMouse = static_cast<System::MouseInputDevice *>(&m_ipInputManager->GetDevice(System::INPUT_DEVICE_MOUSE));

	return info;
}

void Phobos::Engine::Session::ClipMouseCursor()
{
	ConfigInfo_s info = this->GetConfig();

	info.m_pclMouse->ClipToWindow();
}

void Phobos::Engine::Session::UnclipMouseCursor()
{
	ConfigInfo_s info = this->GetConfig();

	info.m_pclMouse->Unclip();	
}

void Phobos::Engine::Session::DisableGameInput()
{
	m_ipInputMapper->Disable();

	if(m_pclPlayerCommandProducer)
		m_pclPlayerCommandProducer->Disable();

	this->UnclipMouseCursor();
}

void Phobos::Engine::Session::EnableGameInput()
{	
	m_ipInputMapper->Enable();

	if(m_pclPlayerCommandProducer)
	{		
		m_pclPlayerCommandProducer->Enable();

		if(m_pclPlayerCommandProducer->IsMouseClipped())
			this->ClipMouseCursor();
	}
}

void Phobos::Engine::Session::OnEvent(System::Event_s &event)
{
	switch (event.m_stSystem.m_eType)
	{
		case System::SYSTEM_QUIT:
			Console::GetInstance().Execute("quit");
			break;

		default:
			break;
	}
}

void Phobos::Engine::Session::Bind(const String_t &devicePathName, const String_t &actionName, const String_t &cmd)
{
	m_ipInputMapper->Bind(devicePathName, actionName, cmd);
}

void Phobos::Engine::Session::Unbind(const String_t &devicePathName, const String_t &actionName)
{
	m_ipInputMapper->Unbind(devicePathName, actionName);
}
