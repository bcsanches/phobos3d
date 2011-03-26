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

#include "PH_InputManager.h"

#include <boost/bind.hpp>

#include <sstream>

#include <PH_Error.h>
#include <PH_Exception.h>
#include <PH_Folders.h>
#include <PH_Kernel.h>
#include <PH_Path.h>

namespace Phobos
{
	static const String_c strKeyboardName_gl("kb");
	static const String_c strMouseName_gl("mouse");
	static const String_c strPadName_gl("pad");

	PH_DEFINE_SINGLETON_VAR(InputManager);

	InputManagerPtr_t InputManager_c::CreateInstance(const String_c &name)
	{
		PH_ASSERT_MSG(!ipInstance_gl, "[InputManager_c::CreateInstance]: Instance already exists");

		ipInstance_gl = InputManager_c::CreateInstanceImpl(name);

		Kernel_c::GetInstance().AddObject(ipInstance_gl, Path_c(PH_SYSTEM_FOLDER));

		return ipInstance_gl;
	}

	InputManagerPtr_t InputManager_c::GetInstance()
	{
		PH_ASSERT_MSG(ipInstance_gl, "[InputManager_c::GetInstance]: Instance does not exists, use CreateInstance");

		return ipInstance_gl;
	}

	void InputManager_c::ReleaseInstance()
	{
		PH_ASSERT_MSG(ipInstance_gl, "[InputManager_c::ReleaseInstance]: Instance does not exists, use CreateInstance");

		ipInstance_gl->RemoveSelf();
		ipInstance_gl.reset();
	}

	InputManager_c::InputManager_c(const String_c &name):
		Node_c(name, PRIVATE_CHILDREN)
	{
	}

	InputManager_c::~InputManager_c(void)
	{
		//empty
	}

	void InputManager_c::Update(void)
	{
		this->PollDevices();

		this->UpdateDevices();
	}

	/**

		Call update on all devices attacheds to input manager.

	*/
	void InputManager_c::UpdateDevices(void)
	{
		for(NodeMap_t::const_iterator it = this->begin(), end = this->end(); it != end; ++it)
		{
			boost::static_pointer_cast<InputDevice_c>(it->second)->Update();
		}
	}

	InputDevicePtr_t InputManager_c::GetDevice(const InputDeviceTypes_e deviceType, UInt_t id)
	{
		String_c name;

		InputManager_c::BuildDeviceName(name, deviceType, id);

		//Because children is private we can control the types that are added
		//so we can safely do a static cast
		return boost::static_pointer_cast<InputDevice_c>(this->GetChild(name));
	}

	InputDevicePtr_t InputManager_c::GetDevice(const InputDeviceTypes_e deviceType)
	{
		//Because children is private we can control the types that are added
		//so we can safely do a static cast
		return boost::static_pointer_cast<InputDevice_c>(this->GetChild(this->GetDeviceTypeName(deviceType)));
	}

	/**



	*/
	void InputManager_c::AttachDevice(InputDevicePtr_t device, UInt_t id)
	{
		//InputDeviceTypes_e	type = device->GetDeviceType();
		//Kernel_c			&kernel = Kernel_c::GetInstance();

		//String_c			tempName;


		//Set the name of the device (concat: name + id)
		//InputManager_c::BuildDeviceName(tempName, type, id);
		//device->SetName(tempName);

		this->AddPrivateChild(device);

		//SEND ATTACH EVENT
		InputManagerEvent_s event(INPUT_MANAGER_EVENT_DEVICE_ATTACHED, device);

		std::for_each(lstListeners.begin(), lstListeners.end(), boost::bind(&InputManagerListener_c::InputManagerEvent, _1, event));
	}


	void InputManager_c::AddListenerToDevice(const String_c &deviceName, InputDeviceListener_c &listener)
	{
		InputDevicePtr_t ptr = boost::static_pointer_cast<InputDevice_c>(this->GetChild(deviceName));

		ptr->AddListener(listener);
	}

	const String_c &InputManager_c::GetDeviceTypeName(InputDeviceTypes_e type)
	{
		switch(type)
		{
			case INPUT_DEVICE_KEYBOARD:
				return(strKeyboardName_gl);

			case INPUT_DEVICE_PAD:
				return(strPadName_gl);

			case INPUT_DEVICE_MOUSE:
				return(strMouseName_gl);

			default:
				PH_ASSERT_MSG(false, "Invalid type for InputManager_c::GetDeviceTypeName");
				{
					std::stringstream stream;
					stream << "Invalid parameter type: " << type;
					PH_RAISE(INVALID_OPERATION_EXCEPTION, "InputManager_c::GetDeviceTypeName", stream.str());
				}
		}
	}

	void InputManager_c::BuildDeviceName(String_c &out, InputDeviceTypes_e type, UInt_t id)
	{
		const String_c &deviceBaseName = GetDeviceTypeName(type);

		std::stringstream stream;

		stream << deviceBaseName;
		stream << id;

		out = stream.str();
	}

	PH_DEFINE_LISTENER_PROCS(InputManager_c, InputManagerListener_c, lstListeners);
}
