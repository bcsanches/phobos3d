/*
Phobos 3d
May 2011
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

#include "Phobos/System/SDL/KeyboardInputDeviceSDL.h"

#include <Phobos/Error.h>
#include <Phobos/Memory.h>

#include "Phobos/System/EventManager.h"
#include "Phobos/System/InputEvent.h"

Phobos::System::InputDevicePtr_t Phobos::System::KeyboardInputDeviceSDL::Create(const String_t &name)
{
	return InputDevicePtr_t(PH_NEW KeyboardInputDeviceSDL(name));
}

Phobos::System::KeyboardInputDeviceSDL::KeyboardInputDeviceSDL(const String_t &name):
	KeyboardInputDevice(name)
{
	EventManager::AddListener(*this, EVENT_TYPE_KEYBOARD);
}

Phobos::System::KeyboardInputDeviceSDL::~KeyboardInputDeviceSDL(void)
{
	//empty
}

void Phobos::System::KeyboardInputDeviceSDL::Update(void)
{
	//empty
}

void Phobos::System::KeyboardInputDeviceSDL::OnEvent(Event_s &event)
{
	PH_ASSERT(event.m_eType == EVENT_TYPE_KEYBOARD);

	InputEvent_s inputEvent;		

	switch(event.m_stKeyboard.m_eType)
	{
		case KEYBOARD_KEY_DOWN:
		case KEYBOARD_KEY_UP:
			inputEvent.m_eType = INPUT_EVENT_BUTTON;
			inputEvent.m_pclDevice = this;
			if(event.m_stKeyboard.m_eType == KEYBOARD_KEY_DOWN)
			{
				inputEvent.m_stButton.m_eState = BUTTON_STATE_DOWN;
				inputEvent.m_stButton.m_fpPression = 1.0f;
			}
			else
			{
				inputEvent.m_stButton.m_eState = BUTTON_STATE_UP;
				inputEvent.m_stButton.m_fpPression = 0.0f;
			}
			inputEvent.m_stButton.m_uId = event.m_stKeyboard.m_u16Code;
			break;

		case KEYBOARD_CHAR:
			inputEvent.m_eType = INPUT_EVENT_CHAR;
			inputEvent.m_pclDevice = this;
			inputEvent.m_stChar.m_u16Char = event.m_stKeyboard.m_u16Code;
			break;

		default:
			return;
	}

	this->DispatchEvent(inputEvent);
}

