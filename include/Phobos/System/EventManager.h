/*
Phobos 3d
January 2010
Copyright (c) 2005-2013 Bruno Sanches  http://code.google.com/p/phobos3d

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef PH_SYSTEM_EVENT_MANAGER_H
#define PH_SYSTEM_EVENT_MANAGER_H

#include <Phobos/Node.h>

#include "Phobos/System/EventListener.h"
#include "Phobos/System/SystemAPI.h"

namespace Phobos
{
	namespace System
	{
		enum EventType_e
		{
			EVENT_TYPE_WINDOW,
			EVENT_TYPE_KEYBOARD,
			EVENT_TYPE_MOUSE,
			EVENT_TYPE_SYSTEM,	

			EVENT_TYPE_NUM
		};

		enum KeyboardEventType_e
		{
			KEYBOARD_KEY_DOWN,
			KEYBOARD_KEY_UP,
			KEYBOARD_CHAR
		};

		enum MouseEventType_e
		{
			MOUSE_MOVE,
			MOUSE_BUTTON_DOWN,
			MOUSE_BUTTON_UP
		};

		enum SystemEventType_e
		{
			SYSTEM_QUIT,
			SYSTEM_ACTIVATE
		};

		struct EventSystem_s
		{
			SystemEventType_e m_eType;

			bool m_fMinimized;
			bool m_fActive;
		};

		struct EventWindow_s
		{

		};

		struct EventKeyboard_s
		{
			KeyboardEventType_e m_eType;

			UInt16_t m_u16Code;
		};

		struct EventMouse_s
		{
			MouseEventType_e m_eType;

			UInt16_t	m_u16ButtonId;

			UInt16_t	m_u16X;
			UInt16_t	m_u16Y;
		};

		struct Event_s
		{
			EventType_e m_eType;

			union
			{
				EventWindow_s 	m_stWindow;
				EventKeyboard_s	m_stKeyboard;
				EventMouse_s	m_stMouse;
				EventSystem_s	m_stSystem;
			};

			void *m_pParam;
		};

		namespace EventManager
		{
			PH_SYSTEM_API void PumpEvents();

			PH_SYSTEM_API void AddListener(EventListener &listener, EventType_e type);
			PH_SYSTEM_API void RemoveListener(EventListener &listener);

			namespace detail
			{
				extern void NotityListeners(Event_s &event);

				extern void DoPumpEvents();
			}
		}
	}
}
#endif