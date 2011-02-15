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

#ifndef PH_INPUT_MANAGER_H
#define PH_INPUT_MANAGER_H

#include "PH_InputAPI.h"

#include <PH_Node.h>
#include <PH_Singleton.h>
#include <PH_Types.h>

#include "PH_InputDefs.h"
#include "PH_InputDevice.h"

namespace Phobos
{		
	enum InputManagerEventType_e
	{
		INPUT_MANAGER_EVENT_DEVICE_ATTACHED,
		INPUT_MANAGER_EVENT_DEVICE_DETACHED
	};

	struct InputManagerEvent_s
	{
		InputManagerEventType_e eType;
		InputDevicePtr_t	ipDevice;

		inline InputManagerEvent_s(InputManagerEventType_e type, InputDevicePtr_t device):
			ipDevice(device),
			eType(type)
		{
		}
	};

	class InputManagerListener_c
	{
		public:
			virtual ~InputManagerListener_c() {};

			virtual void InputManagerEvent(const InputManagerEvent_s &event) = 0;

		public:
			PH_DECLARE_LISTENER_HOOK;
	};

	PH_DECLARE_SINGLETON_PTR(InputManager);	

	class PH_INPUT_API InputManager_c: public Node_c
	{	
		PH_DECLARE_NAMED_SINGLETON_METHODS(InputManager)		

		public:
			// =====================================================
			// PUBLIC METHODS
			// =====================================================
			virtual void Update(void);		
			
			void AddListenerToDevice(const String_c &deviceName,  InputDeviceListener_c &listener);

			InputDevicePtr_t GetDevice(const InputDeviceTypes_e deviceType, UInt_t id);
			InputDevicePtr_t GetDevice(const InputDeviceTypes_e deviceType);

			PH_DECLARE_LISTENER_PROCS(InputManagerListener_c);

		protected:
			// =====================================================
			// PROTECTED METHODS
			// =====================================================
			InputManager_c(const String_c &name);
			~InputManager_c(void);		

			virtual void PollDevices(void) = 0;

			void AttachDevice(InputDevicePtr_t device, UInt_t id);
			void DetachDevice(InputDevicePtr_t device);

		private:
			// =====================================================
			// PRIVATE METHODS
			// =====================================================
			void UpdateDevices(void);					

		public:
			// =====================================================
			// STATIC PUBLIC METHODS
			// =====================================================
			static const String_c &GetDeviceTypeName(InputDeviceTypes_e type);
			static void BuildDeviceName(String_c &out, InputDeviceTypes_e type, UInt_t id);		

		private:
			// =====================================================
			// STATIC PRIVATE METHODS
			// =====================================================
			static InputManagerPtr_t CreateInstanceImpl(const String_c &name);		

		private:
			PH_DECLARE_LISTENER_LIST(InputManagerListener_c, lstListeners);
	};	
}


#endif