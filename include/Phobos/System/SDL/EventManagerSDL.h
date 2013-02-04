/*
Phobos 3d
May 2011
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

#ifndef PH_SYSTEM_EVENT_MANAGER_SDL_H
#define PH_SYSTEM_EVENT_MANAGER_SDL_H

#include "Phobos/System/EventManager.h"

#include <SDL.h>

namespace Phobos
{
	namespace System
	{
		PH_DECLARE_NODE_PTR(EventManagerSDL);

		class PH_SYSTEM_API EventManagerSDL: public EventManager
		{
			public:
				void Update();

			protected:

			private:
				EventManagerSDL(const String_t &name);

				friend class EventManager;

				inline bool IsValidSDLToPhobosKeyCode(SDLKey key);
				bool BuildKeyboardEvent(Event_s &event, SDL_Event &sdl_event);
				void BuildMouseEvent(Event_s &event, SDL_Event& sdl_event);
				void BuildSystemEvent(Event_s &event, SDL_Event& sdl_event);

		};
	}
}

#endif
