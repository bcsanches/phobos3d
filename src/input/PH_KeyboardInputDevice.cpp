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

#include "PH_KeyboardInputDevice.h"

#include <PH_InputActions.h>

#include "PH_InputDefs.h"

namespace Phobos
{
	struct InputKeyName_s
	{
		const Char_t	*pstrzName;
		UInt_t			uKey;
	};

	#define FIRST_ASCII_CHAR ((Char_t) 33)	//!
	#define LAST_ASCII_CHAR ((Char_t) 126)	//~

	#define IS_ASCII_CHAR(VALUE) ((VALUE >= FIRST_ASCII_CHAR) && (VALUE <= LAST_ASCII_CHAR))

	static InputKeyName_s	stInputKeyNames_g[]=
	{
		{"TAB",				KB_TAB},
		{"ENTER",			KB_ENTER},
		{"ESCAPE",			KB_ESCAPE},
		{"SPACE",			KB_SPACE},
		{"BACKSPACE",		KB_BACKSPACE},
		{"UP_ARROW",		KB_UP_ARROW},
		{"DOWN_ARROW",		KB_DOWN_ARROW},
		{"LEFT_ARROW",		KB_LEFT_ARROW},
		{"RIGHT_ARROW",		KB_RIGHT_ARROW},

		{"ALT",				KB_ALT},
		{"CONTROL",			KB_CONTROL},
		{"SHIFT",			KB_SHIFT},

		{"F1",				KB_F1},
		{"F2",				KB_F2},
		{"F3",				KB_F3},
		{"F4",				KB_F4},
		{"F5",				KB_F5},
		{"F6",				KB_F6},
		{"F7",				KB_F7},
		{"F8",				KB_F8},
		{"F9",				KB_F9},
		{"F10",				KB_F10},
		{"F11",				KB_F11},
		{"F12",				KB_F12},

		{"INSERT",			KB_INSERT},
		{"DELETE",			KB_DELETE},
		{"PAGE_DOWN",		KB_PAGE_DOWN},
		{"PAGE_UP",			KB_PAGE_UP},
		{"HOME",			KB_HOME},
		{"END",				KB_END},

		{"KP_HOME",			KB_KP_HOME },
		{"KP_UP_ARROW",		KB_KP_UP_ARROW },
		{"KP_PAGE_UP",		KB_KP_PAGE_UP },
		{"KP_LEFT_ARROW",	KB_KP_LEFT_ARROW },
		{"KP_5",			KB_KP_5 },
		{"KP_RIGHT_ARROW",	KB_KP_RIGHT_ARROW },
		{"KP_END",			KB_KP_END },
		{"KP_DOWN_ARROW",	KB_KP_DOWN_ARROW },
		{"KP_PAGE_DOWN",	KB_KP_PAGE_DOWN },
		{"KP_ENTER",		KB_KP_ENTER },
		{"KP_INSERT",		KB_KP_INSERT },
		{"KP_DELETE",		KB_KP_DELETE },
		{"KP_SLASH",		KB_KP_SLASH },
		{"KP_MINUS",		KB_KP_MINUS },
		{"KP_PLUS",			KB_KP_PLUS },

		{"PAUSE",			KB_PAUSE},

		{"SEMICOLON",		';'},
		{"TILDE",			'`'},
		{"QUOTE",			'\''},

		//Sentinel
		{NULL,				0}
	};	

	KeyboardInputDevice_c::KeyboardInputDevice_c(const String_c &name):
		InputDevice_c(name, INPUT_DEVICE_KEYBOARD)
	{
		//empty
	}

	KeyboardInputDevice_c::~KeyboardInputDevice_c()
	{
		//empty
	}

	bool KeyboardInputDevice_c::TryGetActionName(UInt_t action, String_c &out) const
	{	
		if(IS_ASCII_CHAR(action))
		{			
			out = static_cast<char>(action);

			return true;
		}
		else
		{
			int i = 0;

			for(;stInputKeyNames_g[i].pstrzName != NULL; ++i)
			{
				if(stInputKeyNames_g[i].uKey == action)
				{
					out = stInputKeyNames_g[i].pstrzName;

					return true;
				}
			}
		}	

		return false;
	}

	bool KeyboardInputDevice_c::TryGetActionId(const String_c &name, UInt_t &out) const
	{	
		if(name.length() == 1)
		{
			Char_t ch = name[0];

			if(IS_ASCII_CHAR(ch))
			{
				out = static_cast<UInt_t>(ch);
				return true;
			}
			else
				return false;
		}
		else
		{
			UInt_t i = 0;
			for(;stInputKeyNames_g[i].pstrzName != NULL; ++i)
			{				
				if(name.compare(stInputKeyNames_g[i].pstrzName) == 0)
				{
					out = stInputKeyNames_g[i].uKey;
					return true;
				}
			}

			return false;
		}
	}
}
