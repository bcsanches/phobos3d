/*
Phobos 3d
July 2010
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
#ifndef PH_SYSTEM_KEYBOARD_INPUT_DEVICE_H
#define PH_SYSTEM_KEYBOARD_INPUT_DEVICE_H

#include "Phobos/System/InputDevice.h"

namespace Phobos
{
	namespace System
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
}

#endif