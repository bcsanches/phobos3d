/*
Phobos 3d
  July 2010

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

#ifndef PH_INPUT_DEVICE_H
#define PH_INPUT_DEVICE_H

#include <PH_Node.h>
#include <PH_Types.h>
#include <PH_String.h>

#include "PH_InputDefs.h"
#include "PH_InputDeviceFwd.h"
#include "PH_InputDeviceListener.h"

namespace Phobos
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
	class InputDevice_c: public Node_c
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
				\param out pointer to a String_c to receive the action name

				\return true if the action exists
			*/
			virtual bool TryGetActionName(UInt_t action, String_c &out) const = 0;

			/**
		
				This is the inverse of GetActionName.

				\param name the name of the action to retrieve
				\param out pointer to a IM_UInt_t to receive the id of the action

				\return IM_SUCCESS if the action name is valid for this device
		
			*/
			virtual bool TryGetActionId(const String_c &name, UInt_t &out) const = 0;

			inline const InputDeviceTypes_e GetDeviceType(void) const;

			inline void AddListener(InputDeviceListener_c &listener);

		protected:
			inline InputDevice_c(const String_c &name, InputDeviceTypes_e);
			inline ~InputDevice_c(void);			

			void DispatchEvent(const InputEvent_s &event);

		private:
			const InputDeviceTypes_e eDeviceType;		

			PH_DECLARE_LISTENER_LIST_TYPE(InputDeviceListener_c);
			
			ListenersList_t lstListeners;
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
	inline InputDevice_c::InputDevice_c(const String_c &name, InputDeviceTypes_e type):
		Node_c(name),
		eDeviceType(type)
	{
		//empty
	}

	inline InputDevice_c::~InputDevice_c(void)
	{
		//empty
	}

	/**

		Simple return the type of device

		\return the device type, check IM_InputManager_c::DeviceTypes_e

	*/
	inline const InputDeviceTypes_e InputDevice_c::GetDeviceType(void) const
	{
		return(eDeviceType);
	}

	inline void InputDevice_c::AddListener(InputDeviceListener_c &listener)
	{
		lstListeners.push_back(listener);
	}
}

#endif