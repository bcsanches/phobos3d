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

#include "Phobos/System/SDL/MouseInputDeviceSDL.h"

#include <Phobos/Error.h>
#include <Phobos/Memory.h>

#include "Phobos/System/EventManager.h"
#include "Phobos/System/InputActions.h"
#include "Phobos/System/InputEvent.h"


Phobos::System::InputDevicePtr_t Phobos::System::MouseInputDeviceSDL::Create(const String_t &name)
{
	return InputDevicePtr_t(PH_NEW MouseInputDeviceSDL(name));
}

Phobos::System::MouseInputDeviceSDL::MouseInputDeviceSDL(const String_t &name):
	MouseInputDevice(name),
	fClipToWindow(false),
	fAppActive(true)
{
	EventManager::GetInstance().AddListener(*this, EVENT_TYPE_MOUSE);

	m_clSystemListener.SetOwner(this);
}

Phobos::System::MouseInputDeviceSDL::~MouseInputDeviceSDL(void)
{
	//empty
}

void Phobos::System::MouseInputDeviceSDL::Update(void)
{	    
	//if the mouse is clipped, send update
	if(fClipToWindow && fAppActive)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		SDL_WarpMouse(iWindowCenter[0], iWindowCenter[1]);

		x -= iWindowCenter[0];
		y -= iWindowCenter[1];

		if((x == iPreviousMousePosition[0]) && (y == iPreviousMousePosition[1]))
			return;

		iPreviousMousePosition[0] = x;
		iPreviousMousePosition[1] = y;

		InputEvent_s inputEvent;

		inputEvent.m_eType = INPUT_EVENT_THUMB;
		inputEvent.m_stThumb.m_uId = MOUSE_THUMB;
		inputEvent.m_stThumb.m_fpAxis[0] = static_cast<Float_t>(x);
		inputEvent.m_stThumb.m_fpAxis[1] = static_cast<Float_t>(y);

		inputEvent.m_pclDevice = this;

		this->DispatchEvent(inputEvent);
	}		
}


void Phobos::System::MouseInputDeviceSDL::OnEvent(Event_s &event)
{
	switch(event.m_eType)
	{
		case EVENT_TYPE_MOUSE:
			{
				InputEvent_s inputEvent;

				switch(event.m_stMouse.m_eType)
				{
					case MOUSE_MOVE:
						if(fClipToWindow)
							return;

						inputEvent.m_eType = INPUT_EVENT_THUMB;
						inputEvent.m_stThumb.m_uId = MOUSE_THUMB;
						inputEvent.m_stThumb.m_fpAxis[0] = event.m_stMouse.m_u16X;
						inputEvent.m_stThumb.m_fpAxis[1] = event.m_stMouse.m_u16Y;

						break;

					case MOUSE_BUTTON_DOWN:

						inputEvent.m_eType = INPUT_EVENT_BUTTON;
						inputEvent.m_stButton.m_eState = BUTTON_STATE_DOWN;
						inputEvent.m_stButton.m_fpPression = 1.0f;
						inputEvent.m_stButton.m_uId = event.m_stMouse.m_u16ButtonId;

						break;

					case MOUSE_BUTTON_UP:

						inputEvent.m_eType = INPUT_EVENT_BUTTON;
						inputEvent.m_stButton.m_eState = BUTTON_STATE_UP;
						inputEvent.m_stButton.m_fpPression = 0;
						inputEvent.m_stButton.m_uId = event.m_stMouse.m_u16ButtonId;

						break;

					default:
						return;
				}

				inputEvent.m_pclDevice = this;

				this->DispatchEvent(inputEvent);
			}
			break;

		case EVENT_TYPE_SYSTEM:
			switch(event.m_stSystem.m_eType)
			{
				case SYSTEM_ACTIVATE:
					if(event.m_stSystem.m_fMinimized || !event.m_stSystem.m_fActive)
					{
						fAppActive = false;

						this->Disable();
					}
					else
					{
						fAppActive = true;

						this->Enable();
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

void Phobos::System::MouseInputDeviceSDL::ClipToWindow()
{
	fClipToWindow = true;
	SDL_WM_GrabInput(SDL_GRAB_ON);

	this->Enable();
}

void Phobos::System::MouseInputDeviceSDL::Unclip(void)
{
	SDL_WM_GrabInput(SDL_GRAB_OFF);

	this->Disable();
	fClipToWindow = false;
}

void Phobos::System::MouseInputDeviceSDL::Enable()
{
	if(!fClipToWindow)
		return;

	SDL_Surface *surface = SDL_GetVideoSurface();

	iWindowCenter[0] = surface->w / 2;
	iWindowCenter[1] = surface->h / 2;

	iPreviousMousePosition[0] = 0;
	iPreviousMousePosition[1] = 0;

	SDL_WarpMouse(iWindowCenter[0], iWindowCenter[1]);

	//The warp some times does not get processed before the loop
	//so grab the mouse position and use it as current state
	SDL_GetMouseState(iPreviousMousePosition, iPreviousMousePosition+1);

	iPreviousMousePosition[0] -= iWindowCenter[0];
	iPreviousMousePosition[1] -= iWindowCenter[1];

	SDL_ShowCursor(SDL_DISABLE);		
}
	
void Phobos::System::MouseInputDeviceSDL::Disable()
{
	SDL_ShowCursor(SDL_ENABLE);
}

void Phobos::System::MouseInputDeviceSDL::ShowCursor()
{
	SDL_ShowCursor(SDL_ENABLE);
}

void Phobos::System::MouseInputDeviceSDL::HideCursor()
{
	SDL_ShowCursor(SDL_DISABLE);
}

short Phobos::System::MouseInputDeviceSDL::GetX()
{
	int x;
	SDL_GetMouseState(&x, NULL);

	return x;
}

short Phobos::System::MouseInputDeviceSDL::GetY()
{
	int y;
	SDL_GetMouseState(NULL, &y);

	return y;
}

Phobos::System::MouseInputDeviceSDL::SytemEventListner::SytemEventListner():
	m_pclOwner(NULL)
{
	EventManager::GetInstance().AddListener(*this, EVENT_TYPE_SYSTEM);
}

void Phobos::System::MouseInputDeviceSDL::SytemEventListner::SetOwner(MouseInputDeviceSDL *owner)
{
	m_pclOwner = owner;
}

void Phobos::System::MouseInputDeviceSDL::SytemEventListner::OnEvent(struct Event_s &event)
{
	m_pclOwner->OnEvent(event);
}

