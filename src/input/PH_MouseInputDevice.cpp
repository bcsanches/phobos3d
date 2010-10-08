/*
Phobos 3d
  October 2010

  Copyright (C) 2005-2010 Bruno Crivelari Sanches

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Bruno Crivelari Sanches bcsanches@gmail.com
*/

#include "PH_MouseInputDevice.h"

#include <PH_InputActions.h>

#include "PH_InputDefs.h"

namespace Phobos
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


	MouseInputDevice_c::MouseInputDevice_c(const String_c &name):
		InputDevice_c(name, INPUT_DEVICE_MOUSE)
	{
		//empty
	}

	bool MouseInputDevice_c::TryGetActionName(UInt_t action, String_c &out) const
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

	bool MouseInputDevice_c::TryGetActionId(const String_c &name, UInt_t &out) const
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
}
