#include "W32/PH_WindowW32.h"

#include "PH_Error.h"
#include "PH_Exception.h"

#include <string>

namespace Phobos
{
	static const Char_t WINDOW_CLASS_NAME[] = "PhobosMainWndClass";

	bool WindowW32_c::fWindowClassRegistered = false;

	WindowW32_c::WindowW32_c(const String_c &name):
		Window_c(name),
		hWnd(NULL)
	{
	}

	WindowW32_c::~WindowW32_c(void)
	{	
		if(NULL != hWnd)
			DestroyWindow(hWnd);		
	}

	void WindowW32_c::Open(const String_c &name, const Rect_s<size_t> &rect)
	{		
		this->CreateWindowClass();

		RECT wrect;
		DWORD style = WS_OVERLAPPEDWINDOW;

		wrect.left = rect.tOrigin[0];
		wrect.top = rect.tOrigin[1];
		wrect.right = rect.tWidth;
		wrect.bottom = rect.tHeight;
		AdjustWindowRect(&wrect, style, false);

		if(wrect.left < 0)
		{
			LONG fix = wrect.left * -1;
			wrect.left += fix;
			wrect.right += fix;
		}

		if(wrect.top < 0)
		{
			LONG fix = wrect.top * -1;
			wrect.top += fix;
			wrect.bottom += fix;
		}	

		hWnd = CreateWindow(
			WINDOW_CLASS_NAME,		//Window class name
			name.c_str(),			//Window name
			style,					//window style
			wrect.left,				//initial x position
			wrect.top,				//initial y position
			wrect.right,			//initial width
			wrect.bottom,			//initial height
			GetDesktopWindow(),		//PArent window is desktop
			NULL,					//No menu
			GetModuleHandle(NULL),	//HInstance
			0						//LPARAM
		);		

		if(NULL == hWnd)
		{
			DWORD error = GetLastError();

			std::stringstream stream;
			stream << "Erro creating window: " << error;
			PH_RAISE(NATIVE_API_FAILED_EXCEPTION, "WindowW32_c::Open", stream.str());			
		}
		else
		{			
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG>(this));			

			//if(eventManagerW32)
				//eventManagerW32->SetWnd(hWnd);

			ShowWindow(hWnd, SW_RESTORE);
			UpdateWindow(hWnd);			
		}
	}

	void WindowW32_c::CreateWindowClass(void) 
	{ 
		if(fWindowClassRegistered)
			return;

		WNDCLASSEX	wcx; 	

		ZeroMemory(&wcx, sizeof(wcx));		

		// Fill in the window class structure with parameters 
		// that describe the main window. 
		wcx.cbSize = sizeof(wcx);						// size of structure 
		wcx.style = CS_HREDRAW | CS_VREDRAW;			// redraw if size changes 
		wcx.lpfnWndProc = WindowW32_c::MainWndProc;		// points to window procedure 
		wcx.cbClsExtra = 0;								// no extra class memory 
		wcx.cbWndExtra = 0;								// no extra window memory 
		wcx.hInstance = GetModuleHandle(NULL);			// handle to instance 
		wcx.hIcon = NULL;								// predefined app. icon 
		wcx.hCursor = NULL;								// predefined arrow 
		wcx.hbrBackground = NULL;						// white background brush 
		wcx.lpszMenuName =  NULL;						// name of menu resource 
		wcx.lpszClassName = WINDOW_CLASS_NAME;			// name of window class 
		wcx.hIconSm = NULL;

		// Register the window class. 
		if(!RegisterClassEx(&wcx))
		{
			std::stringstream stream;
			stream << "Error registering window class: " << GetLastError();
			PH_RAISE(NATIVE_API_FAILED_EXCEPTION, "WindowW32_c::CreateWindowClass", stream.str());
		}		

		fWindowClassRegistered = true;
	} 

	size_t WindowW32_c::GetWidth(void) const
	{
		PH_ASSERT_VALID(this);
		PH_ASSERT_VALID(hWnd);

		RECT rect;

		if(GetClientRect(hWnd, &rect))
			return(rect.right - rect.left);
		else
		{
			std::stringstream stream;
			stream << "GetClientRect call failed: " << GetLastError();
			PH_RAISE(NATIVE_API_FAILED_EXCEPTION, "WindowW32_c::GetWidth", stream.str());
		}
	}

	size_t WindowW32_c::GetHeight(void) const
	{
		PH_ASSERT_VALID(this);
		PH_ASSERT_VALID(hWnd);

		RECT rect;

		if(GetClientRect(hWnd, &rect))
			return(rect.bottom - rect.top);
		else
		{
			std::stringstream stream;
			stream << "GetClientRect call failed: " << GetLastError();
			PH_RAISE(NATIVE_API_FAILED_EXCEPTION, "WindowW32_c::GetHeight", stream.str());
		}
	}

	void WindowW32_c::GetRect(Rect_s<size_t> &out)
	{
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
			PH_RAISE(NATIVE_API_FAILED_EXCEPTION, "WindowW32_c::GetRect", stream.str());
		}
	}

	LRESULT WindowW32_c::MainWndMethod(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		//if(pclEventManager)
			 //pclEventManager->OnWindowMessage(hwnd, uMsg, wParam, lParam);		

		return(::DefWindowProc(hwnd, uMsg, wParam, lParam));
	}

	LRESULT CALLBACK WindowW32_c::MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{	
		switch(uMsg)
		{
			case WM_CLOSE:
				PostQuitMessage(0);
				break;
		}			

		WindowW32_c *wnd = reinterpret_cast<WindowW32_c *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));		

		if(wnd)
			return(wnd->MainWndMethod(hwnd, uMsg, wParam, lParam));
		else
			return(::DefWindowProc(hwnd, uMsg, wParam, lParam));
	}

	//fulscreen http://blogs.msdn.com/oldnewthing/archive/2010/04/12/9994016.aspx
}