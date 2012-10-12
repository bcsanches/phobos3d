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

#ifndef PH_MOUSE_INPUT_DEVICE_SDL_H
#define PH_MOUSE_INPUT_DEVICE_SDL_H

#include "PH_MouseInputDevice.h"

#include <PH_EventListener.h>

#include <SDL/SDL.h>

namespace Phobos
{
	class MouseInputDeviceSDL_c: public MouseInputDevice_c, private EventListener_c
	{
		public:
			static InputDevicePtr_t Create(const String_c &name);

			MouseInputDeviceSDL_c(const String_c &name);
			~MouseInputDeviceSDL_c(void);

			virtual void Update(void);

			virtual void ClipToWindow();
			virtual void Unclip(void);

			virtual void ShowCursor();
			virtual void HideCursor();

			virtual short GetX();
			virtual short GetY();

		private:
			virtual void Event(struct Event_s &event);

			void Enable();
			void Disable();

		private:
			class SytemEventListner_c: EventListener_c
			{
				public:
					SytemEventListner_c();

					inline void SetOwner(MouseInputDeviceSDL_c *owner);

				private:
					virtual void Event(struct Event_s &event);

				private:
					MouseInputDeviceSDL_c *pclOwner;
			};

			friend class SytemEventListner_c;

			SytemEventListner_c clSystemListener;

			//RECT		stOldClipRect;
			//bool		fHasClipRect;

			int			iWindowCenter[2];
			int			iPreviousMousePosition[2];
			bool		fClipToWindow;
            bool		fAppActive;
            SDL_Event event;
	};
}

#endif
