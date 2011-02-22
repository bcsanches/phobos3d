#include "SDL/PH_WindowSDL.h"

#include "PH_Error.h"
#include "PH_Exception.h"

#include <string>

namespace Phobos
{
	static const Char_t WINDOW_CLASS_NAME[] = "PhobosMainWndClass";

	//bool WindowSDL_c::fWindowClassRegistered = false;

	WindowPtr_t Window_c::Create(const String_c &name)
	{
		return WindowPtr_t(new WindowSDL_c(name));
	}

	WindowSDL_c::WindowSDL_c(const String_c &name):
		Window_c(name)//,
		//hWnd(NULL)
	{
	}

	WindowSDL_c::~WindowSDL_c(void)
	{
		//if(NULL != hWnd)
			//DestroyWindow(hWnd);

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

		//PH_ASSERT_VALID(hWnd);


        /*
		RECT rect;

		if(GetClientRect(hWnd, &rect))
			return(rect.right - rect.left);
		else
		{
			std::stringstream stream;
			stream << "GetClientRect call failed: " << GetLastError();
			PH_RAISE(NATIVE_API_FAILED_EXCEPTION, "WindowSDL_c::GetWidth", stream.str());
		}
		*/
	}

	size_t WindowSDL_c::GetHeight(void) const
	{
		PH_ASSERT_VALID(this);

		return rect.tHeight;

		//PH_ASSERT_VALID(hWnd);

        /*
		RECT rect;

		if(GetClientRect(hWnd, &rect))
			return(rect.bottom - rect.top);
		else
		{
			std::stringstream stream;
			stream << "GetClientRect call failed: " << GetLastError();
			PH_RAISE(NATIVE_API_FAILED_EXCEPTION, "WindowSDL_c::GetHeight", stream.str());
		}
		*/
	}

	void WindowSDL_c::GetRect(Rect_s<size_t> &out)
	{

	    out = rect;

	    /*
		RECT rect;
		if(GetClientRect(hWnd, &rect))
		{
			out.tOrigin[0] = rect.left;
			out.tOrigin[1] = rect.top;

			out.tHeight = rect.bottom - rect.top;
			out.tWidth = rect.right - rect.left;
		}
		else
		{
			std::stringstream stream;
			stream << "GetClientRect call failed: " << GetLastError();
			PH_RAISE(NATIVE_API_FAILED_EXCEPTION, "WindowSDL_c::GetRect", stream.str());
		}
		*/
	}

	void *WindowSDL_c::GetHandler() const
	{
		//return hWnd;

		return NULL;
	}

	void WindowSDL_c::SetEventManager(EventManagerPtr_t manager)
	{
		//ipEventManager = boost::static_pointer_cast<EventManagerW32_c>(manager);
	}

    /*
	LRESULT WindowSDL_c::MainWndMethod(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if(ipEventManager)
			ipEventManager->OnWindowMessage(hwnd, uMsg, wParam, lParam);

		return(::DefWindowProc(hwnd, uMsg, wParam, lParam));
	}



	LRESULT CALLBACK WindowSDL_c::MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch(uMsg)
		{
			case WM_CLOSE:
				PostQuitMessage(0);
				break;
		}

		WindowSDL_c *wnd = reinterpret_cast<WindowSDL_c *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

		if(wnd)
			return(wnd->MainWndMethod(hwnd, uMsg, wParam, lParam));
		else
			return(::DefWindowProc(hwnd, uMsg, wParam, lParam));
	}

	//fulscreen http://blogs.msdn.com/oldnewthing/archive/2010/04/12/9994016.aspx

	*/
}
