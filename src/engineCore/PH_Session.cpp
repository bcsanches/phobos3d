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

#include "PH_Session.h"

#include <Phobos/System/InputActions.h>
#include <Phobos/System/InputDevice.h>
#include <Phobos/System/InputEvent.h>
#include <Phobos/System/InputManager.h>
#include <Phobos/System/MouseInputDevice.h>

#include "Gui/PH_Form.h"
#include "Gui/PH_Manager.h"

#include "PH_Console.h"
#include "PH_IClient.h"
#include "PH_IPlayerCommandProducer.h"

#define CONSOLE_KEY '`'

PH_DEFINE_DEFAULT_SINGLETON(Phobos::Session);

Phobos::Session::Session():
	CoreModule("Session"),
	m_fIgnoreConsoleKey(false),
	m_pclPlayerCommandProducer(NULL),
	m_pclClient(NULL),
	m_pclForm(NULL)
{
	System::InputManager::CreateInstance("InputManager").AddListener(*this);

	m_ipInputMapper = System::InputMapper::Create("InputMapper", Console::GetInstance());
	m_ipInputMapper->Disable();

	this->AddChild(*m_ipInputMapper);
}

Phobos::Session::~Session()
{
	System::InputManager::ReleaseInstance();
}

void Phobos::Session::OnInputManagerEvent(const System::InputManagerEvent_s &event)
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

void Phobos::Session::OnInputEvent(const System::InputEvent_s &event)
{
	Phobos::Console &console = Phobos::Console::GetInstance();

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

				Gui::Form *newForm = NULL;
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
				Gui::Manager::GetInstance().DisableInput();
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

void Phobos::Session::OnFixedUpdate()
{
	System::InputManager::GetInstance().Update();

	Phobos::Console &console = Phobos::Console::GetInstance();

	console.FlushCommandBuffer();

	if(!console.IsActive())
	{
		if((m_pclPlayerCommandProducer) && (m_pclClient))
		{
			IPlayerCmdPtr_t cmd = m_pclPlayerCommandProducer->CreateCmd();
			m_pclClient->SetPlayerCmd(cmd);
		}
	}
}

void Phobos::Session::SetPlayerCommandProducer(IPlayerCommandProducer *commandProducer)
{	
	Phobos::Console &console = Phobos::Console::GetInstance();

	if(m_pclPlayerCommandProducer)
	{
		m_pclPlayerCommandProducer->Disable();

		if(!console.IsActive() && !m_pclForm)
			this->DisableGameInput();
	}

	m_pclPlayerCommandProducer = commandProducer;
	
	if(m_pclPlayerCommandProducer)
	{
		if(m_pclClient)
		{
			//Sets a default cmd
			IPlayerCmdPtr_t cmd = m_pclPlayerCommandProducer->CreateCmd();
			m_pclClient->SetPlayerCmd(cmd);
		}
			
		if(!console.IsActive() && (!m_pclForm))
		{
			this->EnableGameInput();			
		}
	}
	else if(!console.IsActive())
	{
		this->UnclipMouseCursor();
	}
}

void Phobos::Session::SetClient(Phobos::IClient *client)
{
	m_pclClient = client;
}

void Phobos::Session::CloseConsole()
{
	Console &console = Console::GetInstance();
	if(!console.IsActive())
		return;

	//Close console
	console.ToggleConsole();

	//now give input focus to game or current form

	if(m_pclForm)
	{
		Gui::Manager::GetInstance().EnableInput();
		this->GetConfig().m_pclMouse->HideCursor();
	}
	else
	{
		this->EnableGameInput();
	}
}

void Phobos::Session::SetGuiForm(Gui::Form *newForm)
{
	Console &console = Console::GetInstance();

	if((m_pclForm == NULL) && (!console.IsActive()))
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
		if(m_pclForm != NULL)
		{
			Gui::Manager::GetInstance().EnableInput();

			this->GetConfig().m_pclMouse->HideCursor();
		}
		else
		{
			Gui::Manager::GetInstance().DisableInput();
			this->GetConfig().m_pclMouse->ShowCursor();
			
			this->EnableGameInput();
		}
	}
}

Phobos::Session::ConfigInfo_s Phobos::Session::GetConfig()
{
	ConfigInfo_s info;
	
	info.m_pclMouse = static_cast<System::MouseInputDevice *>(&System::InputManager::GetInstance().GetDevice(System::INPUT_DEVICE_MOUSE));

	return info;
}

void Phobos::Session::ClipMouseCursor()
{
	ConfigInfo_s info = this->GetConfig();

	info.m_pclMouse->ClipToWindow();
}

void Phobos::Session::UnclipMouseCursor()
{
	ConfigInfo_s info = this->GetConfig();

	info.m_pclMouse->Unclip();	
}

void Phobos::Session::DisableGameInput()
{
	m_ipInputMapper->Disable();

	if(m_pclPlayerCommandProducer)
		m_pclPlayerCommandProducer->Disable();

	this->UnclipMouseCursor();
}

void Phobos::Session::EnableGameInput()
{	
	if(m_pclPlayerCommandProducer)
	{
		m_ipInputMapper->Enable();

		m_pclPlayerCommandProducer->Enable();

		if(m_pclPlayerCommandProducer->IsMouseClipped())
			this->ClipMouseCursor();
	}
}
