/*
Phobos 3d
  May 2010

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

#include "SDL/PH_InputManagerSDL.h"

#include <PH_Kernel.h>

#include "PH_InputDefs.h"

#include "SDL/PH_KeyboardInputDeviceSDL.h"
#include "SDL/PH_MouseInputDeviceSDL.h"

namespace Phobos
{
	InputManagerPtr_t InputManager_c::CreateInstanceImpl(const String_c &name)
	{
		return InputManagerPtr_t(new InputManagerSDL_c(name));
	}

	InputManagerSDL_c::InputManagerSDL_c(const String_c &name):
		InputManager_c(name),
		fPooled(false)
	{
		Kernel_c::GetInstance().LogMessage("[InputManagerSDL] Created.");
	}

	void InputManagerSDL_c::PollDevices(void)
	{
		if(fPooled)
			return;

		fPooled = true;
		this->AttachDevice(KeyboardInputDeviceSDL_c::Create(InputManager_c::GetDeviceTypeName(INPUT_DEVICE_KEYBOARD)), 0);
		this->AttachDevice(MouseInputDeviceSDL_c::Create(InputManager_c::GetDeviceTypeName(INPUT_DEVICE_MOUSE)), 0);
	}
}
