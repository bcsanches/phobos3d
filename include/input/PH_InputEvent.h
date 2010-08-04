#ifndef PH_INPUT_EVENT_H
#define PH_INPUT_EVENT_H

#include "PH_InputDefs.h"
#include "PH_InputDeviceFwd.h"

namespace Phobos
{
	/**
		
		This structure hold info about a input event: the listener should check
		the eType filed before using the union. The structures on the union will be 
		filled according to eType field.
		
	*/
	struct InputEvent_s
	{
		InputEventType_e	eType;
		InputDevicePtr_t	ipDevice;

		union
		{
			InputEventButton_s 	stButton;
			InputEventChar_s	stChar;	
			InputEventThumb_s	stThumb;
		};
	};
}

#endif