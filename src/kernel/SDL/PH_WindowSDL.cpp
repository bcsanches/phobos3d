#include "SDL/PH_WindowSDL.h"

#include "PH_Error.h"
#include "PH_Exception.h"

#include <string>

namespace Phobos
{
	//static const Char_t WINDOW_CLASS_NAME[] = "PhobosMainWndClass";

	WindowPtr_t Window_c::Create(const String_c &name)
	{
		return WindowPtr_t(new WindowSDL_c(name));
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

	    SDL_WM_SetCaption(name.c_str(), NULL);

        #ifdef PH_WIN32
            SDL_SetVideoMode(rect.tWidth, rect.tHeight, 32, 0);
        #else
            SDL_SetVideoMode(rect.tWidth, rect.tHeight, 32, SDL_OPENGL);
        #endif

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

	void WindowSDL_c::SetEventManager(EventManagerPtr_t manager)
	{
		ipEventManager = boost::static_pointer_cast<EventManagerSDL_c>(manager);
	}
}
