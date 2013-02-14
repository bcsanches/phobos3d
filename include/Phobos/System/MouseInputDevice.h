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

#ifndef PH_SYSTEM_MOUSE_INPUT_DEVICE_H
#define PH_SYSTEM_MOUSE_INPUT_DEVICE_H

#include "Phobos/System/InputDevice.h"

namespace Phobos
{
	namespace System
	{
		PH_DECLARE_NODE_PTR(MouseInputDevice);
	
		class MouseInputDevice: public InputDevice
		{		
			public:
				virtual bool TryGetActionName(UInt_t action, String_t &out) const override;
				virtual bool TryGetActionId(const String_t &name, UInt_t &out) const override;
			
				virtual void ClipToWindow() = 0;
				virtual void Unclip(void) = 0;

				virtual void ShowCursor() = 0;
				virtual void HideCursor() = 0;

				virtual short GetX() = 0;
				virtual short GetY() = 0;

			protected:
				MouseInputDevice(const String_t &name);		
		};
	}
}

#endif
