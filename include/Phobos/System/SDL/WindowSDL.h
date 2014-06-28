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

#ifndef PH_SYSTEM_WINDOW_SDL_H
#define PH_SYSTEM_WINDOW_SDL_H

#include "Phobos/System/Window.h"

//#include <windows.h>
#include <SDL.h>
#include <SDL_syswm.h>

namespace Phobos
{
	namespace System
	{
		class PH_SYSTEM_API WindowSDL: public Window
		{
			public:			
				~WindowSDL();

				virtual void Open(	const String_t &name, const UIntSize_t &size, unsigned int flags, void *parentWindow = NULL) override;

				virtual size_t GetWidth(void) const override;
				virtual size_t GetHeight(void) const override;

				virtual UIntSize_t GetSize() const override;

				virtual void *GetHandler() const override;

				virtual bool HasGLContext() override;

				virtual void SetParentWindow(void *parentWindow) override;

			protected:			
				WindowSDL(const String_t &name);
		
			private:			
				friend WindowPtr_t Window::Create(const String_t &name);	

				bool m_fOpenGLContext;
		};
	}
}

#endif
