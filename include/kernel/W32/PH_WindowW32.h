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
