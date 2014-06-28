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

#include "Phobos/System/SDL/WindowSDL.h"

#include <Phobos/Error.h>
#include <Phobos/Exception.h>
#include <Phobos/Memory.h>

#include <string>

#include <SDL.h>
#include <SDL_syswm.h>

Phobos::System::WindowPtr_t Phobos::System::Window::Create(const String_t &name)
{
	return WindowPtr_t(PH_NEW WindowSDL(name));
}

Phobos::System::WindowSDL::WindowSDL(const String_t &name):
	Window(name),
	m_fOpenGLContext(false)
{
}

Phobos::System::WindowSDL::~WindowSDL(void)
{
        SDL_Quit();
}

void Phobos::System::WindowSDL::Open(const String_t &name, const UIntSize_t &size, unsigned int flags, void *parentWindow)
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_EnableUNICODE(1);

	unsigned int sdlFlags = 0;

	if (flags & WND_OPENGL_CONTEXT)
	{
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

		sdlFlags |= SDL_OPENGL;
		m_fOpenGLContext = true;
	}

	if (flags & WND_FULL_SCREEN)
		sdlFlags |= SDL_FULLSCREEN;

    #ifdef PH_WIN32
		
		HWND parent = reinterpret_cast<HWND>(parentWindow);
		if(parent)
			flags |= SDL_NOFRAME | SDL_RESIZABLE;

        SDL_SetVideoMode(size.m_tWidth, size.m_tHeight, 32, flags);
		
		if(parent)
		{
			this->SetParentWindow(parentWindow);
		}

    #else
        SDL_SetVideoMode(rect.tWidth, rect.tHeight, 0, flags);
    #endif

    SDL_WM_SetCaption(name.c_str(), NULL);
}

void Phobos::System::WindowSDL::SetParentWindow(void *parentWindow)
{
#ifdef PH_WIN32	
	HWND sdlHwnd = reinterpret_cast<HWND>(this->GetHandler());
	HWND parent = reinterpret_cast<HWND>(parentWindow);

	SetParent(sdlHwnd, parent);

	if (parent)
	{
		//not 100% trusted, but give us an extra check
		if (!IsWindow(parent))
		{
			PH_RAISE(INVALID_PARAMETER_EXCEPTION, "[WindowSDL::Open]", "Invalid parent window handle");
		}

		RECT parentRect;
		GetClientRect(parent, &parentRect);

		//SetWindowPos( sdlHwnd, parent, 0, 0, parentRec.right, parentRec.bottom, ( 0 /*SWP_NOSIZE*/ | SWP_SHOWWINDOW ));
		MoveWindow(sdlHwnd, 0, 0, parentRect.right, parentRect.bottom, true);

		SDL_SetVideoMode(parentRect.right - parentRect.left, parentRect.bottom - parentRect.top, 32, SDL_NOFRAME | SDL_RESIZABLE);
	}	
#else
	#error "Not implemented"
#endif
}

size_t Phobos::System::WindowSDL::GetWidth(void) const
{
	PH_ASSERT_VALID(this);

	return SDL_GetVideoSurface()->w;
}

size_t Phobos::System::WindowSDL::GetHeight(void) const
{
	PH_ASSERT_VALID(this);

	return SDL_GetVideoSurface()->h;
}

Phobos::UIntSize_t Phobos::System::WindowSDL::GetSize() const
{
	auto surface = SDL_GetVideoSurface();

	return UIntSize_t(surface->w, surface->h);
}

void *Phobos::System::WindowSDL::GetHandler() const
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

bool Phobos::System::WindowSDL::HasGLContext()
{
	return m_fOpenGLContext;
}	
