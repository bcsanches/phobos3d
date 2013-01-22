/*
Phobos 3d
February 2010
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


#ifndef PH_WINDOW_W32_H
#define PH_WINDOW_W32_H

#include "PH_Window.h"
#include "W32/PH_EventManagerW32.h"

#include <windows.h>

namespace Phobos
{
	class PH_KERNEL_API WindowW32_c: public Window_c
	{
		public:
			void Open(	const String_c &name,
						const Rect_s<size_t> &rect);

			size_t GetWidth(void) const;
			size_t GetHeight(void) const;

			void GetRect(Rect_s<size_t> &out);

			virtual void *GetHandler() const;

			virtual bool HasGLContext();

			virtual void SetEventManager(EventManagerPtr_t manager);

		private:
			WindowW32_c(const String_c &name);
			~WindowW32_c();

		private:
			static LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		private:
			void CreateWindowClass();
			LRESULT MainWndMethod(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		private:
			static bool fWindowClassRegistered;

			friend static WindowPtr_t Window_c::Create(const String_c &name);

		private:
			HWND hWnd;
			EventManagerW32Ptr_t ipEventManager;
	};
}

#endif
