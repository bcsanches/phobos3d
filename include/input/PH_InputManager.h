#ifndef PH_INPUT_MANAGER_H
#define PH_INPUT_MANAGER_H

#include "PH_InputAPI.h"

#include <PH_Node.h>
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

	class InputManager_c;
	typedef boost::intrusive_ptr<InputManager_c> InputManagerPtr_t;

	class PH_INPUT_API InputManager_c: public Node_c
	{	
		public:
			static InputManagerPtr_t CreateInstance(const String_c &name);
			static void ReleaseInstance();
			static InputManagerPtr_t GetInstance();	

		public:
			// =====================================================
			// PUBLIC METHODS
			// =====================================================
			virtual void Update(void);		
			
			void AddListenerToDevice(const String_c &deviceName,  InputDeviceListener_c &listener);

			InputDevicePtr_t GetDevice(const InputDeviceTypes_e deviceType, UInt_t id);

			void AddListener(InputManagerListener_c &listener);
			void RemoveListener(InputManagerListener_c &listener);

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
			static InputManagerPtr_t ipInstance_gl;

		private:
			PH_DECLARE_LISTENER_LIST_TYPE(InputManagerListener_c);
			ListenersList_t lstListeners;
	};	
}


#endif