/*
Phobos 3d
October 2012
Copyright (c) 2005-2012 Bruno Sanches  http://code.google.com/p/phobos3d

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

#include <PH_InputActions.h>
#include <PH_InputDevice.h>
#include <PH_InputEvent.h>
#include <PH_InputManager.h>
#include <PH_Kernel.h>
#include <PH_MouseInputDevice.h>

#include "Gui/PH_Form.h"
#include "Gui/PH_Manager.h"

#include "PH_Console.h"
#include "PH_IClient.h"
#include "PH_IPlayerCommandProducer.h"

#define CONSOLE_KEY '`'

PH_DEFINE_DEFAULT_SINGLETON(Phobos::Session);

Phobos::Session_c::Session_c():
	CoreModule_c("Session"),
	fIgnoreConsoleKey(false),
	pclPlayerCommandProducer(NULL),
	pclClient(NULL),
	pclForm(NULL)
{
	InputManager_c::CreateInstance("InputManager").AddListener(*this);

	ipInputMapper = InputMapper_c::Create("InputMapper", Console_c::GetInstance());
	ipInputMapper->Disable();

	this->AddChild(*ipInputMapper);
}

Phobos::Session_c::~Session_c()
{
	InputManager_c::ReleaseInstance();
}

void Phobos::Session_c::InputManagerEvent(const InputManagerEvent_s &event)
{
	std::stringstream stream;

	switch(event.eType)
	{
		case INPUT_MANAGER_EVENT_DEVICE_ATTACHED:
			stream << "[Console_c::InputManagerEvent] Device " << event.rclDevice.GetName() << " attached.";
			if(event.rclDevice.GetDeviceType() == INPUT_DEVICE_KEYBOARD)
				event.rclDevice.AddListener(*this);
			break;

		case INPUT_MANAGER_EVENT_DEVICE_DETACHED:
			stream << "[Console_c::InputManagerEvent] Device " << event.rclDevice.GetName() << " detached.";
			break;
	}

	Kernel_c::GetInstance().LogMessage(stream.str());
}

void Phobos::Session_c::InputEvent(const InputEvent_s &event)
{
	Phobos::Console_c &console = Phobos::Console_c::GetInstance();

	if((event.eType == INPUT_EVENT_BUTTON) && (event.stButton.eState == Phobos::BUTTON_STATE_DOWN))
	{		
		if(event.stButton.uId == Phobos::KB_ESCAPE)
		{
			if(console.IsActive())
			{
				this->CloseConsole();				
			}
			else
			{
				//No console, so someone else must handle this

				Gui::Form_c *newForm = NULL;
				EscAction::Enum action = EscAction::IGNORE_ESC;

				if(pclForm)
					action = pclForm->HandleEsc(newForm);
				else if(pclClient)
					action = pclClient->HandleEsc(newForm);

				if(action == EscAction::SET_GUI)
				{
					this->SetForm(newForm);
				}

			}
		}
		else if((event.stButton.uId == CONSOLE_KEY || event.stButton.uId == '\'') && !console.IsActive())
		{
			fIgnoreConsoleKey = true;
			console.ToggleConsole();

			if(pclForm)
			{
				Gui::Manager_c::GetInstance().DisableInput();
				this->GetConfig().pclMouse->ShowCursor();
			}
			else
			{			
				this->DisableGameInput();
			}
		}
	}

	if(console.IsActive())
	{
		if(event.eType == INPUT_EVENT_CHAR)
		{			
			if(fIgnoreConsoleKey && (event.stButton.uId == CONSOLE_KEY || event.stButton.uId == '\''))
			{
				fIgnoreConsoleKey = false;
				return;
			}
			fIgnoreConsoleKey = false;
		}		

		console.HandleInputEvent(event);	
	}
}

void Phobos::Session_c::OnFixedUpdate()
{
	InputManager_c::GetInstance().Update();

	Phobos::Console_c &console = Phobos::Console_c::GetInstance();

	console.FlushCommandBuffer();

	if(!console.IsActive())
	{
		if((pclPlayerCommandProducer) && (pclClient))
		{
			IPlayerCmdPtr_t cmd = pclPlayerCommandProducer->CreateCmd();
			pclClient->SetPlayerCmd(cmd);
		}
	}
}

void Phobos::Session_c::SetPlayerCommandProducer(IPlayerCommandProducer_c *commandProducer)
{	
	if(pclPlayerCommandProducer)
	{
		pclPlayerCommandProducer->Disable();		
	}

	pclPlayerCommandProducer = commandProducer;

	Phobos::Console_c &console = Phobos::Console_c::GetInstance();
	if(pclPlayerCommandProducer)
	{
		if(pclClient)
		{
			//Sets a default cmd
			IPlayerCmdPtr_t cmd = pclPlayerCommandProducer->CreateCmd();
			pclClient->SetPlayerCmd(cmd);
		}
			
		if(!console.IsActive() && (!pclForm))
		{
			pclPlayerCommandProducer->Enable();

			if(pclPlayerCommandProducer->IsMouseClipped())
				this->ClipMouseCursor();
		}
	}
	else if(!console.IsActive())
	{
		this->UnclipMouseCursor();
	}
}

void Phobos::Session_c::SetClient(Phobos::IClient_c *client)
{
	pclClient = client;
}

void Phobos::Session_c::CloseConsole()
{
	Console_c &console = Console_c::GetInstance();
	if(!console.IsActive())
		return;

	//Close console
	console.ToggleConsole();

	//now give input focus to game or current form

	if(pclForm)
	{
		Gui::Manager_c::GetInstance().EnableInput();
		this->GetConfig().pclMouse->HideCursor();
	}
	else
	{
		this->EnableGameInput();
	}
}

void Phobos::Session_c::SetForm(Gui::Form_c *newForm)
{
	Console_c &console = Console_c::GetInstance();

	if((pclForm == NULL) && (!console.IsActive()))
	{
		//no previous form, so game active, disable it
		this->DisableGameInput();
	}

	pclForm = newForm;

	if(!console.IsActive())
	{
		if(pclForm != NULL)
		{
			Gui::Manager_c::GetInstance().EnableInput();

			this->GetConfig().pclMouse->HideCursor();
		}
		else
		{
			Gui::Manager_c::GetInstance().DisableInput();
			this->GetConfig().pclMouse->ShowCursor();

			this->EnableGameInput();
		}
	}
}

Phobos::Session_c::ConfigInfo_s Phobos::Session_c::GetConfig()
{
	ConfigInfo_s info;
	
	info.pclMouse = static_cast<MouseInputDevice_c *>(&InputManager_c::GetInstance().GetDevice(INPUT_DEVICE_MOUSE));

	return info;
}

void Phobos::Session_c::ClipMouseCursor()
{
	ConfigInfo_s info = this->GetConfig();

	info.pclMouse->ClipToWindow();
}

void Phobos::Session_c::UnclipMouseCursor()
{
	ConfigInfo_s info = this->GetConfig();

	info.pclMouse->Unclip();	
}

void Phobos::Session_c::DisableGameInput()
{
	ipInputMapper->Disable();

	if(pclPlayerCommandProducer)
		pclPlayerCommandProducer->Disable();

	this->UnclipMouseCursor();
}

void Phobos::Session_c::EnableGameInput()
{
	ipInputMapper->Enable();

	if(pclPlayerCommandProducer)
	{
		pclPlayerCommandProducer->Enable();

		if(pclPlayerCommandProducer->IsMouseClipped())
			this->ClipMouseCursor();
	}
}