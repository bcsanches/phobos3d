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

#ifndef PH_WINDOW_SDL_H
#define PH_WINDOW_SDL_H

#include "PH_Window.h"
#include "SDL/PH_EventManagerSDL.h"

//#include <windows.h>
#include <SDL.h>
#include <SDL_syswm.h>

namespace Phobos
{
	class PH_KERNEL_API WindowSDL_c: public Window_c
	{
		public:			
			~WindowSDL_c();

			void Open(	const String_c &name, const Rect_s<size_t> &rect, void *parentWindow = NULL);

			size_t GetWidth(void) const;
			size_t GetHeight(void) const;

			void GetRect(Rect_s<size_t> &out);

			virtual void *GetHandler() const;

			virtual bool HasGLContext();

		private:			
			WindowSDL_c(const String_c &name);
		
		private:			
			friend WindowPtr_t Window_c::Create(const String_c &name);

		private:
			EventManagerSDLPtr_t ipEventManager;

			Rect_s<size_t> rect;
	};
}

#endif
