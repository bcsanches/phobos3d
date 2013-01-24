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

#include <PH_Node.h>

#include "Phobos/System/EventListener.h"
#include "Phobos/System/EventManagerFwd.h"
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
			SystemEventType_e eType;

			bool fMinimized;
			bool fActive;
		};

		struct EventWindow_s
		{

		};

		struct EventKeyboard_s
		{
			KeyboardEventType_e eType;

			UInt16_t u16Code;
		};

		struct EventMouse_s
		{
			MouseEventType_e eType;

			UInt16_t	u16ButtonId;

			UInt16_t	u16X;
			UInt16_t	u16Y;
		};

		struct Event_s
		{
			EventType_e eType;

			union
			{
				EventWindow_s 	stWindow;
				EventKeyboard_s	stKeyboard;
				EventMouse_s	stMouse;
				EventSystem_s	stSystem;
			};

			void *pParam;
		};

		class PH_SYSTEM_API EventManager_c: public Node_c
		{
			public:	
				static EventManager_c &CreateInstance(const String_c &name);
				static void ReleaseInstance();
				static EventManager_c &GetInstance();	
				static const String_c &GetDefaultName();

			public:
				virtual void Update() = 0;

				void AddListener(EventListener_c &listener, EventType_e type);
				void RemoveListener(EventListener_c &listener);

			protected:
				static EventManagerPtr_t CreateInstanceImpl(const String_c &name);			

				EventManager_c(const String_c &name);

				bool IsListenersListEmpty(EventType_e type);
				void NotityListeners(Event_s &event);

			private:			
				static const String_c DEFAULT_NAME;
				static EventManagerPtr_t ipInstance_gl;

			private:
				PH_DECLARE_LISTENER_LIST_TYPE(EventListener_c);

				ListenersList_t arlstListeners[EVENT_TYPE_NUM];

		};
	}
}
#endif