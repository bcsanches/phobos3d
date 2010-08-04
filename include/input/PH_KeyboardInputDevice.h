#ifndef PH_KEYBOARD_INPUT_DEVICE_H
#define PH_KEYBOARD_INPUT_DEVICE_H

#include "PH_InputDevice.h"

namespace Phobos
{

	class KeyboardInputDevice_c: public InputDevice_c
	{		
		public:
			// =====================================================
			// PUBLIC METHODS
			// =====================================================
			virtual bool TryGetActionName(UInt_t action, String_c &out) const;
			virtual bool TryGetActionId(const String_c &name, UInt_t &out) const;

		protected:
			KeyboardInputDevice_c(const String_c &name);
			~KeyboardInputDevice_c(void);					
	};	
}

#endif