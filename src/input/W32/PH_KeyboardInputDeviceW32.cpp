/*
Phobos 3d
  May 2010

  Copyright (C) 2005-2010 Bruno Crivelari Sanches

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

#include "W32/PH_KeyboardInputDeviceW32.h"

#include <PH_Error.h>
#include <PH_EventManager.h>

#include "PH_InputEvent.h"

namespace Phobos
{	
	InputDevicePtr_t KeyboardInputDeviceW32_c::Create(const String_c &name)
	{
		return InputDevicePtr_t(new KeyboardInputDeviceW32_c(name));
	}

	KeyboardInputDeviceW32_c::KeyboardInputDeviceW32_c(const String_c &name):
		KeyboardInputDevice_c(name)
	{
		EventManager_c::GetInstance()->AddListener(*this, EVENT_TYPE_KEYBOARD);
	}
	
	KeyboardInputDeviceW32_c::~KeyboardInputDeviceW32_c(void)
	{
		//empty
	}

	void KeyboardInputDeviceW32_c::Update(void)
	{
		//empty
	}

	void KeyboardInputDeviceW32_c::Event(struct Event_s &event)
	{
		PH_ASSERT(event.eType == EVENT_TYPE_KEYBOARD);

		InputEvent_s inputEvent;

		switch(event.stKeyboard.eType)
		{
			case KEYBOARD_KEY_DOWN:
			case KEYBOARD_KEY_UP:
				inputEvent.eType = INPUT_EVENT_BUTTON;
				inputEvent.ipDevice = this;
				if(event.stKeyboard.eType == KEYBOARD_KEY_DOWN)
				{
					inputEvent.stButton.eState = BUTTON_STATE_DOWN;
					inputEvent.stButton.fpPression = 1.0f;
				}
				else
				{
					inputEvent.stButton.eState = BUTTON_STATE_UP;
					inputEvent.stButton.fpPression = 0.0f;
				}
				inputEvent.stButton.uId = event.stKeyboard.u16Code;
				break;

			case KEYBOARD_CHAR:
				inputEvent.eType = INPUT_EVENT_CHAR;
				inputEvent.ipDevice = this;
				inputEvent.stChar.u16Char = event.stKeyboard.u16Code;
				break;			
		}

		this->DispatchEvent(inputEvent);
	}
}