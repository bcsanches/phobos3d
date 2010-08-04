#ifndef PH_KEYBOARD_INPUT_DEVICE_W32_H
#define PH_KEYBOARD_INPUT_DEVICE_W32_H

#include "PH_KeyboardInputDevice.h"

#include <PH_EventListener.h>

namespace Phobos
{
	class KeyboardInputDeviceW32_c: public KeyboardInputDevice_c, private EventListener_c
	{	
		public:
			static InputDevicePtr_t Create(const String_c &name);


			virtual void Update(void);

		private:
			KeyboardInputDeviceW32_c(const String_c &name);
			~KeyboardInputDeviceW32_c(void);

			virtual void Event(struct Event_s &event);
	};	
}

#endif