#include "W32/PH_InputManagerW32.h"

#include "PH_InputDefs.h"

#include "W32/PH_KeyboardInputDeviceW32.h"

namespace Phobos
{
	static InputManagerPtr_t CreateInstanceLocalImpl(const String_c &name)
	{
		return InputManagerPtr_t(new InputManagerW32_c(name));
	}

	InputManagerPtr_t InputManager_c::CreateInstanceImpl(const String_c &name)
	{
		return CreateInstanceLocalImpl(name);
	}

	InputManagerW32_c::InputManagerW32_c(const String_c &name):
		InputManager_c(name),
		fPooled(false)
	{
	}

	void InputManagerW32_c::PollDevices(void)
	{
		if(fPooled)
			return;

		fPooled = true;
		this->AttachDevice(KeyboardInputDeviceW32_c::Create(InputManager_c::GetDeviceTypeName(INPUT_DEVICE_KEYBOARD)), 0);
	}
}
