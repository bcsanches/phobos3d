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

#include "Phobos/System/MouseInputDevice.h"

#include "Phobos/System/InputActions.h"
#include "Phobos/System/InputDefs.h"

namespace Phobos
{
	namespace System
	{
		namespace
		{
			struct ActionInfo_s
			{
				const char	*pstrzName;
				UInt_t	uId;
			};

			static const ActionInfo_s stActions_gl[] = 
			{
				{"THUMB", MOUSE_THUMB},
				{"LBUTTON", MOUSE_LBUTTON},
				{"RBUTTON", MOUSE_RBUTTON},
				{"MBUTTON", MOUSE_MBUTTON},
				{NULL, 0}
			};
		}
	}
}


Phobos::System::MouseInputDevice::MouseInputDevice(const String_t &name):
	InputDevice(name, INPUT_DEVICE_MOUSE)
{
	//empty
}

bool Phobos::System::MouseInputDevice::TryGetActionName(UInt_t action, String_t &out) const
{
	for(int i = 0;stActions_gl[i].pstrzName; ++i)
	{
		if(stActions_gl[i].uId == action)
		{				
			out = stActions_gl[i].pstrzName;
			return true;
		}
	}

	return false;
}

bool Phobos::System::MouseInputDevice::TryGetActionId(const String_t &name, UInt_t &out) const
{
	for(int i = 0;stActions_gl[i].pstrzName; ++i)
	{			
		if(name.compare(stActions_gl[i].pstrzName) == 0)
		{
			out = stActions_gl[i].uId;
			return true;
		}
	}

	return false;
}
