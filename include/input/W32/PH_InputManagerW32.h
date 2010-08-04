#ifndef PH_INPUT_MANAGER_W32_H
#define PH_INPUT_MANAGER_W32_H

#include "PH_InputManager.h"

namespace Phobos
{
	class InputManagerW32_c: public InputManager_c
	{
		protected:
			virtual void PollDevices(void);

		private:
			InputManagerW32_c(const String_c &name);

			friend static InputManagerPtr_t CreateInstanceLocalImpl(const String_c &name);

		private:
			bool fPooled;
	};
}

#endif