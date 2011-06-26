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

#ifndef PH_MOUSE_INPUT_DEVICE_W32_H
#define PH_MOUSE_INPUT_DEVICE_W32_H

#include "PH_MouseInputDevice.h"

#include <PH_EventListener.h>

#include <windows.h>

namespace Phobos
{
	class MouseInputDeviceW32_c: public MouseInputDevice_c, private EventListener_c
	{		
		protected:
			MouseInputDeviceW32_c(const String_c &name);
			~MouseInputDeviceW32_c(void);

		public:
			static InputDevicePtr_t Create(const String_c &name);

			virtual void Update(void);

			virtual void AcquireCapture(void *window);
			virtual void ReleaseCapture(void);

			virtual void ClipToWindow(void *window);
			virtual void Unclip(void);

		private:
			virtual void Event(struct Event_s &event);

			void Enable();
			void Disable();

		private:			
			class SytemEventListner_c: EventListener_c
			{
				public:
					SytemEventListner_c();

					inline void SetOwner(MouseInputDeviceW32_c *owner);

				private:
					virtual void Event(struct Event_s &event);

				private:
					MouseInputDeviceW32_c *pclOwner;
			};

			friend class SytemEventListner_c;

			SytemEventListner_c clSystemListener;

			RECT		stOldClipRect;
			bool		fHasClipRect;

			int			iWindowCenter[2];
			HWND		hClipWindow;
			bool		fAppActive;
	};
}

#endif