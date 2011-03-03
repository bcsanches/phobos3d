#ifndef PH_EVENT_MANAGER_SDL_H
#define PH_EVENT_MANAGER_SDL_H

#include "PH_EventManager.h"

#include <SDL/SDL.h>

namespace Phobos
{
	PH_DECLARE_NODE_PTR(EventManagerSDL);

	class PH_KERNEL_API EventManagerSDL_c: public EventManager_c
	{
		public:
			void Update();

		protected:

		private:
			EventManagerSDL_c(const String_c &name);

			friend EventManagerPtr_t CreateInstanceLocalImpl(const String_c &name);

            inline bool IsValidSDLToPhobosKeyCode(SDLKey key);
            bool BuildKeyboardEvent(Event_s &event, SDL_Event &sdl_event);
            void BuildMouseEvent(Event_s &event, SDL_Event& sdl_event);
            void BuildSystemEvent(Event_s &event, SDL_Event& sdl_event);

	};
}

#endif
