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

#ifndef PH_SYSTEM_INPUT_DEVICE_H
#define PH_SYSTEM_INPUT_DEVICE_H

#include <Phobos/Node.h>
#include <Phobos/Types.h>
#include <Phobos/String.h>

#include "Phobos/System/InputDefs.h"
#include "Phobos/System/InputDeviceFwd.h"
#include "Phobos/System/InputDeviceListener.h"

namespace Phobos
{
	namespace System
	{
		/**
			\addtogroup input

			InputDevices are used to represent a generic input system that can be expanded
			to all devices types.
		*/

		/**
			\ingroup input	

			This class is used to represent a input device, its has info like types of
			inputs that a device support and info about the device type.

			All devices should derive from this class.

		*/
		class InputDevice: public Node
		{		
			public:
				/**		
					Some devices need to be updated every frame to check the device status,
					others receive events.

					This method is called automatic by IM_InputManager_c (Update on 
					IM_InputManager_c should be called).
				*/
				virtual void Update(void) = 0;		

				/**		
					This return a string name to a button id from this device.

					For example, calling with IM_KB_UP_ARROW (define in #IM_InputKeys_e) it will put "UP_ARROW" on out.

					\param action the action to get the name
					\param out pointer to a String_t to receive the action name

					\return true if the action exists
				*/
				virtual bool TryGetActionName(UInt_t action, String_t &out) const = 0;

				/**
		
					This is the inverse of GetActionName.

					\param name the name of the action to retrieve
					\param out pointer to a IM_UInt_t to receive the id of the action

					\return IM_SUCCESS if the action name is valid for this device
		
				*/
				virtual bool TryGetActionId(const String_t &name, UInt_t &out) const = 0;

				inline const InputDeviceTypes_e GetDeviceType(void) const;

				PH_DECLARE_LISTENER_PROCS(InputDeviceListener);	

			protected:
				inline InputDevice(const String_t &name, InputDeviceTypes_e);
				inline ~InputDevice(void);			

				void DispatchEvent(const InputEvent_s &event);

			private:
				const InputDeviceTypes_e m_eDeviceType;		

				PH_DECLARE_LISTENER_LIST_TYPE(InputDeviceListener);
			
				ListenersList_t m_lstListeners;
		};

		// =====================================================
		// INLINE IMPLEMENTATION
		// =====================================================

		/**

			Default constructor, the derived class should specify the type of device
			that this class represents.

			This value cant be changed.

			\param type the type of device that this class represents

		*/
		inline InputDevice::InputDevice(const String_t &name, InputDeviceTypes_e type):
			Node(name),
			m_eDeviceType(type)
		{
			//empty
		}

		inline InputDevice::~InputDevice(void)
		{
			//empty
		}

		/**

			Simple return the type of device

			\return the device type, check IM_InputManager_c::DeviceTypes_e

		*/
		inline const InputDeviceTypes_e InputDevice::GetDeviceType(void) const
		{
			return(m_eDeviceType);
		}		
	}
}

#endif