/*
Phobos 3d
July 2010
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

#ifndef PH_SYSTEM_INPUT_MANAGER_H
#define PH_SYSTEM_INPUT_MANAGER_H

#include "Phobos/System/SystemAPI.h"

#include <Phobos/Node.h>
#include <Phobos/Singleton.h>
#include <Phobos/Types.h>

#include "Phobos/System/InputDefs.h"
#include "Phobos/System/InputDeviceListener.h"

namespace Phobos
{
	namespace System
	{
		class InputDevice;
		class InputDeviceListener;

		enum InputManagerEventType_e
		{
			INPUT_MANAGER_EVENT_DEVICE_ATTACHED,
			INPUT_MANAGER_EVENT_DEVICE_DETACHED
		};

		struct InputManagerEvent_s
		{
			InputManagerEventType_e m_eType;
			InputDevice				&m_rclDevice;

			inline InputManagerEvent_s(InputManagerEventType_e type, InputDevice &device):
				m_eType(type),
				m_rclDevice(device)

			{
				//empty
			}
		};

		class InputManagerListener
		{
			public:
				virtual ~InputManagerListener() {};

				virtual void OnInputManagerEvent(const InputManagerEvent_s &event) = 0;

			public:
				PH_DECLARE_LISTENER_HOOK;
		};

		PH_DECLARE_SINGLETON_PTR(InputManager);

		class PH_SYSTEM_API InputManager: public Node
		{
			PH_DECLARE_NAMED_SINGLETON_METHODS(InputManager)

			public:
				// =====================================================
				// PUBLIC METHODS
				// =====================================================
				virtual void Update(void);

				void AddListenerToDevice(const String_t &deviceName,  InputDeviceListener &listener);

				InputDevice &GetDevice(const InputDeviceTypes_e deviceType, UInt_t id);
				InputDevice &GetDevice(const InputDeviceTypes_e deviceType);

				PH_DECLARE_LISTENER_PROCS(InputManagerListener);

			protected:
				// =====================================================
				// PROTECTED METHODS
				// =====================================================
				InputManager(const String_t &name);
				~InputManager(void);

				virtual void PollDevices(void) = 0;

				void AttachDevice(InputDevice &device, UInt_t id);
				void DetachDevice(InputDevice &device);

			private:
				// =====================================================
				// PRIVATE METHODS
				// =====================================================
				void UpdateDevices(void);

			public:
				// =====================================================
				// STATIC PUBLIC METHODS
				// =====================================================
				static const String_t &GetDeviceTypeName(InputDeviceTypes_e type);
				static void BuildDeviceName(String_t &out, InputDeviceTypes_e type, UInt_t id);

			private:
				// =====================================================
				// STATIC PRIVATE METHODS
				// =====================================================
				static InputManagerPtr_t CreateInstanceImpl(const String_t &name);

			private:
				PH_DECLARE_LISTENER_LIST(InputManagerListener, m_lstListeners);
		};
	}
}


#endif
