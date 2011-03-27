/*
Phobos 3d
  October 2010

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

#include "SDL/PH_MouseInputDeviceSDL.h"

#include <PH_Error.h>
#include <PH_EventManager.h>
#include <PH_InputActions.h>

#include "PH_InputEvent.h"

namespace Phobos
{
	InputDevicePtr_t MouseInputDeviceSDL_c::Create(const String_c &name)
	{
		return InputDevicePtr_t(new MouseInputDeviceSDL_c(name));
	}

	MouseInputDeviceSDL_c::MouseInputDeviceSDL_c(const String_c &name):
		MouseInputDevice_c(name),
		//fHasClipRect(false),
		fAppActive(true)
	{
		EventManager_c::GetInstance()->AddListener(*this, EVENT_TYPE_MOUSE);

		clSystemListener.SetOwner(this);
	}

	MouseInputDeviceSDL_c::~MouseInputDeviceSDL_c(void)
	{
		//empty
	}

	void MouseInputDeviceSDL_c::Update(void)
	{
	    /*
		//if the mouse is clipped, send update
		if(hClipWindow && fAppActive)
		{
			POINT		cursorPos;

			::GetCursorPos (&cursorPos);
			::SetCursorPos(iWindowCenter[0], iWindowCenter[1]);

			InputEvent_s inputEvent;

			inputEvent.eType = INPUT_EVENT_THUMB;
			inputEvent.stThumb.uId = MOUSE_THUMB;
			inputEvent.stThumb.fpAxis[0] = (Float_t)(cursorPos.x - iWindowCenter[0]);
			inputEvent.stThumb.fpAxis[1] = (Float_t)(cursorPos.y - iWindowCenter[1]);

			inputEvent.ipDevice = InputDevicePtr_t(this);

			this->DispatchEvent(inputEvent);
		}
		*/

		/*
            como não preciso travar o mouse no centro axo q isso não é
            necessário nessa classe.
        */
	}


	void MouseInputDeviceSDL_c::Event(struct Event_s &event)
	{
		switch(event.eType)
		{
			case EVENT_TYPE_MOUSE:
				{
					InputEvent_s inputEvent;

					switch(event.stMouse.eType)
					{
						case MOUSE_MOVE:

							inputEvent.eType = INPUT_EVENT_THUMB;
							inputEvent.stThumb.uId = MOUSE_THUMB;
							inputEvent.stThumb.fpAxis[0] = event.stMouse.u16X;
							inputEvent.stThumb.fpAxis[1] = event.stMouse.u16Y;

							break;

						case MOUSE_BUTTON_DOWN:

							inputEvent.eType = INPUT_EVENT_BUTTON;
							inputEvent.stButton.eState = BUTTON_STATE_DOWN;
							inputEvent.stButton.fpPression = 1.0f;
							inputEvent.stButton.uId = event.stMouse.u16ButtonId;

							break;

						case MOUSE_BUTTON_UP:

							inputEvent.eType = INPUT_EVENT_BUTTON;
							inputEvent.stButton.eState = BUTTON_STATE_UP;
							inputEvent.stButton.fpPression = 0;
							inputEvent.stButton.uId = event.stMouse.u16ButtonId;

							break;

						default:
							return;
					}

					inputEvent.ipDevice = InputDevicePtr_t(this);

					this->DispatchEvent(inputEvent);
				}
				break;

			case EVENT_TYPE_SYSTEM:
				switch(event.stSystem.eType)
				{
					case SYSTEM_ACTIVATE:
						if(event.stSystem.fMinimized || !event.stSystem.fActive)
						{
							fAppActive = false;

							this->ClipToWindow(0);
						}
						else
						{
							fAppActive = true;

							this->Unclip();
						}
						break;

                    default:
                        break;
				}
				break;

            default:
                break;
		}
	}

	void MouseInputDeviceSDL_c::AcquireCapture(void *window)
	{
	    //empty
	}

	void MouseInputDeviceSDL_c::ReleaseCapture(void)
	{
	    //empty
	}

	void MouseInputDeviceSDL_c::ClipToWindow(void *window)
	{
	    SDL_WM_GrabInput(SDL_GRAB_ON);
	}

	void MouseInputDeviceSDL_c::Unclip(void)
	{
	    SDL_WM_GrabInput(SDL_GRAB_OFF);
	}

	MouseInputDeviceSDL_c::SytemEventListner_c::SytemEventListner_c():
		pclOwner(NULL)
	{
		EventManager_c::GetInstance()->AddListener(*this, EVENT_TYPE_SYSTEM);
	}

	void MouseInputDeviceSDL_c::SytemEventListner_c::SetOwner(MouseInputDeviceSDL_c *owner)
	{
		pclOwner = owner;
	}

	void MouseInputDeviceSDL_c::SytemEventListner_c::Event(struct Event_s &event)
	{
		pclOwner->Event(event);
	}

}
