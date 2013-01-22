/*
Phobos 3d
January 2010
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

#ifndef PH_INPUT_ACTIONS_H
#define PH_INPUT_ACTIONS_H

namespace Phobos
{
	namespace System
	{
		/**

			\ingroup input

			This enum has value to every KEY that a keyboard device can map.

			The basic keys ranging from 'a' to 'z', numbers ('0' to '9') are 
			mapped using thei ascii value.

		*/
		enum KeyboardKeys_e
		{
			KB_BACKSPACE	= 8,
			KB_TAB = 9,
			KB_CLEAR = 12,
			KB_ENTER	= 13,
			KB_PAUSE = 19,
			KB_ESCAPE = 27,
			KB_SPACE	= 32,

			KB_UP_ARROW = 128,
			KB_DOWN_ARROW = 129,
			KB_LEFT_ARROW = 130,
			KB_RIGHT_ARROW = 131,

			KB_ALT = 132,
			KB_CONTROL = 133,
			KB_SHIFT	= 134,

			KB_F1 = 135,
			KB_F2 = 136,
			KB_F3 = 137,
			KB_F4 = 138,
			KB_F5 = 139,
			KB_F6 = 140,
			KB_F7 = 141,
			KB_F8 = 142,
			KB_F9 = 143,
			KB_F10 = 144,
			KB_F11 = 145,
			KB_F12 = 146,

			KB_INSERT = 147,
			KB_DELETE = 148,
			KB_PAGE_DOWN = 149,
			KB_PAGE_UP = 150,
			KB_HOME = 151,
			KB_END = 152,

			//Keypad
			KB_KP_HOME = 160,
			KB_KP_UP_ARROW = 161,
			KB_KP_PAGE_UP = 162,
			KB_KP_LEFT_ARROW = 163,
			KB_KP_5 = 164,
			KB_KP_RIGHT_ARROW = 165,
			KB_KP_END = 166,
			KB_KP_DOWN_ARROW = 167,
			KB_KP_PAGE_DOWN = 168,
			KB_KP_ENTER = 169,
			KB_KP_INSERT = 170,
			KB_KP_DELETE = 171,
			KB_KP_SLASH = 172,
			KB_KP_MINUS = 173,
			KB_KP_PLUS = 174,
			KB_KP_MUL = 175,

			KB_NUM_LOCK = 176,
			KB_SCROLL_LOCK = 177,

			KB_LEFT_SHIFT = 178,
			KB_RIGHT_SHIFT = 179,
			KB_LEFT_CONTROL = 180,
			KB_RIGHT_CONTROL = 181,

			KB_CAPS_LOCK = 182,
			KB_PRINT_SCREEN = 183,

			KB_MAX_KEYS = 255
		};

		enum MouseInput_e
		{
			MOUSE_LBUTTON = 1,
			MOUSE_RBUTTON,
			MOUSE_MBUTTON,
			MOUSE_THUMB
		};
	}

}

#endif