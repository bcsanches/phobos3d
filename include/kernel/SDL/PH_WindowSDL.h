#ifndef PH_WINDOW_SDL_H
#define PH_WINDOW_SDL_H

#include "PH_Window.h"
//#include "W32/PH_EventManagerW32.h"

//#include <windows.h>
#include <SDL/SDL.h>
#include <SDL/SDL_syswm.h>

namespace Phobos
{
	class PH_KERNEL_API WindowSDL_c: public Window_c
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
			WindowSDL_c(const String_c &name);
			~WindowSDL_c();

		private:
			//static LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		private:
			//void CreateWindowClass();
			//LRESULT MainWndMethod(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		private:
			//static bool fWindowClassRegistered;

			friend WindowPtr_t Window_c::Create(const String_c &name);

		private:
			//EventManagerW32Ptr_t ipEventManager;

			Rect_s<size_t> rect;
	};
}

#endif
