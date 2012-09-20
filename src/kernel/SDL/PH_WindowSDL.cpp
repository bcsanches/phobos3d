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

#include "SDL/PH_WindowSDL.h"

#include "PH_Error.h"
#include "PH_Exception.h"
#include "PH_Memory.h"

#include <string>

namespace Phobos
{
	//static const Char_t WINDOW_CLASS_NAME[] = "PhobosMainWndClass";

	WindowPtr_t Window_c::Create(const String_c &name)
	{
		return WindowPtr_t(PH_NEW WindowSDL_c(name));
	}

	WindowSDL_c::WindowSDL_c(const String_c &name):
		Window_c(name)
	{
	}

	WindowSDL_c::~WindowSDL_c(void)
	{
            SDL_Quit();
	}

	void WindowSDL_c::Open(const String_c &name, const Rect_s<size_t> &rect)
	{

	    SDL_Init(SDL_INIT_VIDEO);

	    SDL_EnableUNICODE(1);

            #ifdef PH_WIN32
                SDL_SetVideoMode(rect.tWidth, rect.tHeight, 32, 0);
            #else
                SDL_SetVideoMode(rect.tWidth, rect.tHeight, 0, SDL_OPENGL);
            #endif

            #ifdef PH_LINUX
                SDL_GL_SetAttribute( SDL_GL_RED_SIZE,           8  );
                SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE,         8  );
                SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE,          8  );
                SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE,         16 );
            #endif

            SDL_WM_SetCaption(name.c_str(), NULL);

	    this->rect = rect;

	}

	size_t WindowSDL_c::GetWidth(void) const
	{
		PH_ASSERT_VALID(this);

		return rect.tWidth;
	}

	size_t WindowSDL_c::GetHeight(void) const
	{
		PH_ASSERT_VALID(this);

		return rect.tHeight;
	}

	void WindowSDL_c::GetRect(Rect_s<size_t> &out)
	{
	    out = rect;
	}

	void *WindowSDL_c::GetHandler() const
	{
	    #ifdef PH_WIN32

            SDL_SysWMinfo pInfo;
            SDL_VERSION(&pInfo.version);
            SDL_GetWMInfo(&pInfo);

            return pInfo.window;

        #else
            return NULL;
        #endif
	}

	bool WindowSDL_c::HasGLContext()
	{
	    #ifdef PH_WIN32
            return false;
        #else
            return true;
        #endif
	}	
}
