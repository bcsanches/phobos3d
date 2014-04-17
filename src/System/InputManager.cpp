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

#include "Phobos/System/InputManager.h"

#include <sstream>

#include <Phobos/Error.h>
#include <Phobos/Exception.h>
#include <Phobos/Folders.h>
#include <Phobos/ObjectManager.h>
#include <Phobos/Path.h>

#include "Phobos/System/InputDevice.h"


namespace
{
	static const Phobos::String_t strKeyboardName_gl("kb");
	static const Phobos::String_t strMouseName_gl("mouse");
	static const Phobos::String_t strPadName_gl("pad");	
}

Phobos::System::InputManager::InputManager(const String_t &name):
	Node(name, NodeFlags::PRIVATE_CHILDREN)
{
}

Phobos::System::InputManager::~InputManager(void)
{
	//empty
}

void Phobos::System::InputManager::Update(void)
{
	this->UpdateDevices();
}

/**

	Call update on all devices attacheds to input manager.

*/
void Phobos::System::InputManager::UpdateDevices(void)
{
	for(auto it : *this)
	{		
		static_cast<InputDevice *>(it.second)->Update();		
	}	
}

void Phobos::System::InputManager::Accept(std::function<void(InputDevice &)> visitor)
{
	for (auto it : *this)
	{
		visitor(*static_cast<InputDevice *>(it.second));		
	}
}

Phobos::System::InputDevice &Phobos::System::InputManager::GetDevice(const InputDeviceTypes_e deviceType, UInt_t id)
{
	String_t name;

	InputManager::BuildDeviceName(name, deviceType, id);

	//Because children is private we can control the types that are added
	//so we can safely do a static cast
	return static_cast<InputDevice &>(this->GetChild(name));
}

Phobos::System::InputDevice &Phobos::System::InputManager::GetDevice(const InputDeviceTypes_e deviceType)
{
	//Because children is private we can control the types that are added
	//so we can safely do a static cast
	return static_cast<InputDevice &>(this->GetChild(this->GetDeviceTypeName(deviceType)));
}

/**



*/
void Phobos::System::InputManager::AttachDevice(InputDevice &device, UInt_t id)
{	
	//Set the name of the device (concat: name + id)
	//InputManager::BuildDeviceName(tempName, type, id);
	//device->SetName(tempName);

	this->AddPrivateChild(device);

	//SEND ATTACH EVENT
	InputManagerEvent_s event(INPUT_MANAGER_EVENT_DEVICE_ATTACHED, device);

	for(auto &listener: m_lstListeners)
	{
		listener.OnInputManagerEvent(event);
	}	
}


void Phobos::System::InputManager::AddListenerToDevice(const String_t &deviceName, InputDeviceListener &listener)
{
	auto &device = static_cast<InputDevice &>(this->GetChild(deviceName));

	device.AddListener(listener);
}

const Phobos::String_t &Phobos::System::InputManager::GetDeviceTypeName(InputDeviceTypes_e type)
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
			PH_ASSERT_MSG(false, "Invalid type for InputManager::GetDeviceTypeName");
			{
				std::stringstream stream;
				stream << "Invalid parameter type: " << type;
				PH_RAISE(INVALID_OPERATION_EXCEPTION, "InputManager::GetDeviceTypeName", stream.str());
			}
	}
}

void Phobos::System::InputManager::BuildDeviceName(String_t &out, InputDeviceTypes_e type, UInt_t id)
{
	const String_t &deviceBaseName = GetDeviceTypeName(type);

	std::stringstream stream;

	stream << deviceBaseName;
	stream << id;

	out = stream.str();
}

PH_DEFINE_LISTENER_PROCS(Phobos::System::InputManager, Phobos::System::InputManagerListener, m_lstListeners);

