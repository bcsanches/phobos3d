#ifndef PH_EVENT_MANAGER_W32_H
#define PH_EVENT_MANAGER_W32_H

#include "PH_EventManager.h"

#include <windows.h>

namespace Phobos
{
	PH_DECLARE_NODE_PTR(EventManagerW32);

	class PH_KERNEL_API EventManagerW32_c: public EventManager_c
	{
		public:
			void Update();

			void OnWindowMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		protected:

		private:
			EventManagerW32_c(const String_c &name);

			friend static EventManagerPtr_t CreateInstanceLocalImpl(const String_c &name);
	};
}

#endif
