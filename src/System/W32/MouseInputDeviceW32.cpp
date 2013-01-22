/*
Phobos 3d
October 2010
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
#include "W32/PH_MouseInputDeviceW32.h"

#include <PH_Error.h>
#include <PH_EventManager.h>
#include <PH_InputActions.h>

#include "PH_InputEvent.h"

namespace Phobos
{
	InputDevicePtr_t MouseInputDeviceW32_c::Create(const String_c &name)
	{
		return InputDevicePtr_t(new MouseInputDeviceW32_c(name));
	}

	MouseInputDeviceW32_c::MouseInputDeviceW32_c(const String_c &name):
		MouseInputDevice_c(name),
		fHasClipRect(false),
		hClipWindow(NULL),
		fAppActive(true)
	{
		EventManager_c::GetInstance()->AddListener(*this, EVENT_TYPE_MOUSE);

		clSystemListener.SetOwner(this);
	}

	MouseInputDeviceW32_c::~MouseInputDeviceW32_c(void)
	{
		//empty		
	}

	void MouseInputDeviceW32_c::Update(void)
	{
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
	}


	void MouseInputDeviceW32_c::Event(struct Event_s &event)
	{			
		switch(event.eType)
		{
			case EVENT_TYPE_MOUSE:
				{
					InputEvent_s inputEvent;
					switch(event.stMouse.eType)
					{
						case MOUSE_MOVE:
							if(hClipWindow)
								return;

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
							this->Disable();
						}
						else
						{
							fAppActive = true;
							this->Enable();
						}
						break;
				}
				break;
		}
	}

	void MouseInputDeviceW32_c::AcquireCapture(void *window)
	{
		::SetCapture((HWND)window);		
	}

	void MouseInputDeviceW32_c::ReleaseCapture(void)
	{
		::ReleaseCapture();
	}

	void MouseInputDeviceW32_c::ClipToWindow(void *window)
	{
		PH_ASSERT_VALID(window);

		hClipWindow = (HWND)window;

		this->Enable();
	}

	void MouseInputDeviceW32_c::Unclip(void)
	{
		this->Disable();
		
		hClipWindow = NULL;
	}

	void MouseInputDeviceW32_c::Enable()
	{
		if(!hClipWindow)
			return;

		int			width, height;
		RECT		windowRect;

		width = GetSystemMetrics (SM_CXSCREEN);
		height = GetSystemMetrics (SM_CYSCREEN);

		GetWindowRect ( hClipWindow, &windowRect);
		if (windowRect.left < 0)
			windowRect.left = 0;
		if (windowRect.top < 0)
			windowRect.top = 0;
		if (windowRect.right >= width)
			windowRect.right = width-1;
		if (windowRect.bottom >= height-1)
			windowRect.bottom = height-1;

		iWindowCenter[0] = (windowRect.right + windowRect.left)/2;
		iWindowCenter[1] = (windowRect.top + windowRect.bottom)/2;

		SetCursorPos (iWindowCenter[0], iWindowCenter[1]);

		this->AcquireCapture(hClipWindow);

		fHasClipRect = ::GetClipCursor(&stOldClipRect) ? true : false;
		::ClipCursor (&windowRect);

		while (ShowCursor (FALSE) >= 0);
	}

	void MouseInputDeviceW32_c::Disable()
	{
		::ClipCursor(fHasClipRect ? &stOldClipRect : NULL);
		this->ReleaseCapture();

		while(ShowCursor(TRUE) < 0);

		fHasClipRect = false;
	}

	MouseInputDeviceW32_c::SytemEventListner_c::SytemEventListner_c():
		pclOwner(NULL)
	{
		EventManager_c::GetInstance()->AddListener(*this, EVENT_TYPE_SYSTEM);
	}

	void MouseInputDeviceW32_c::SytemEventListner_c::SetOwner(MouseInputDeviceW32_c *owner)
	{
		pclOwner = owner;
	}

	void MouseInputDeviceW32_c::SytemEventListner_c::Event(struct Event_s &event)
	{
		pclOwner->Event(event);
	}

}