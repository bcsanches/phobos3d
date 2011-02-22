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

	    SDL_WM_SetCaption(name.c_str(), NULL);

	    SDL_SetVideoMode(rect.tWidth, rect.tHeight, 32, 0);

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
		return NULL;
	}

	void WindowSDL_c::SetEventManager(EventManagerPtr_t manager)
	{
		//ipEventManager = boost::static_pointer_cast<EventManagerW32_c>(manager);
	}
}
