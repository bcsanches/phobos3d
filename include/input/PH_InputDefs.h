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

#ifndef PH_INPUT_DEFS_H
#define PH_INPUT_DEFS_H

#include <PH_Types.h>

namespace Phobos
{
	/**
		
		This enum is used to identify a device type. Every InputDevice class
		implements a method that returns a value from this enum.
		
	*/
	enum InputDeviceTypes_e
	{
		INPUT_DEVICE_KEYBOARD = 0,
		INPUT_DEVICE_MOUSE,
		INPUT_DEVICE_PAD,		

		INPUT_DEVICE_TYPES_NUM	
	};

	/**
		
		This enum is used to identify a type of input event sent by a input
		device to its listeners.
		
	*/
	enum InputEventType_e
	{
		INPUT_EVENT_BUTTON,
		INPUT_EVENT_CHAR,
		INPUT_EVENT_THUMB	
	};

	/**
		
		Input event sent by a device, normally a joystick.
		This event represents a joystick thumb stick.
		
	*/
	struct InputEventThumb_s
	{
		UInt_t 	uId;
		Float_t	fpAxis[2];
	};

	/**
		
		Input event sent by an input device, normally a keyboard.
		This event is a 16 bit unicode char.
		
	*/
	struct InputEventChar_s
	{
		//Always down
		UInt16_t u16Char;
	};

	/**
		
		This enum represents states of a button event.
		Every button on a joystick, mouse or keyboard has one of these states.								
	*/
	enum InputEventButtonState_e
	{
		//!indicates that the button was pressed 
		//!(and in the last state it was not pressed).
		BUTTON_STATE_DOWN,

		//!indicates that the button was pressed and now is released.
		BUTTON_STATE_UP,

		//!indicates that the button is already pressed and now its pression changed.
		BUTTON_STATE_UPDATE
	};

	/**
		
		This is the event sent when InputEvent type is INPUT_EVENT_BUTTON.		
		
	*/
	struct InputEventButton_s
	{
		//!The id of the button, can use this with the device to get the button name						
		UInt_t				uId;
			
		//!Pression of the button. If 1.0 button is released, if 0 button is completely pressed						
		Float_t 			fpPression;
				
		//!The state of the button.						
		InputEventButtonState_e	eState;
	};	


}

#endif