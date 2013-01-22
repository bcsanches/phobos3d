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

#include "Phobos/System/InputMapper.h"

#include <PH_ContextUtils.h>
#include <PH_Exception.h>
#include <PH_Kernel.h>
#include <PH_Memory.h>
#include <PH_Path.h>

#include "Phobos/System/InputEvent.h"
#include "Phobos/System/InputManager.h"

Phobos::System::InputMapperPtr_t Phobos::System::InputMapper_c::Create(const String_c &name, IContext_c &context)
{
	return InputMapperPtr_t(PH_NEW InputMapper_c(name, context));
}

Phobos::System::InputMapper_c::InputMapper_c(const String_c &name, IContext_c &context):
	Node_c(name),
	rclContext(context),
	fDisable(false),
	cmdBind("bind"),
	cmdUnbind("unbind"),
	cmdPushButton("pushButton"),
	cmdReleaseButton("releaseButton")
{
	InputManager_c &manager = InputManager_c::GetInstance();

	manager.AddListener(*this);

	cmdBind.SetProc(PH_CONTEXT_CMD_BIND(&InputMapper_c::CmdBind, this));
	cmdUnbind.SetProc(PH_CONTEXT_CMD_BIND(&InputMapper_c::CmdUnbind, this));
	cmdPushButton.SetProc(PH_CONTEXT_CMD_BIND(&InputMapper_c::CmdPushButton, this));
	cmdReleaseButton.SetProc(PH_CONTEXT_CMD_BIND(&InputMapper_c::CmdReleaseButton, this));

	rclContext.AddContextCmd(cmdBind);
	rclContext.AddContextCmd(cmdUnbind);
	rclContext.AddContextCmd(cmdPushButton);
	rclContext.AddContextCmd(cmdReleaseButton);
}

Phobos::System::InputMapper_c::~InputMapper_c(void)
{
	//empty
}

void Phobos::System::InputMapper_c::Execute(const std::string &cmd)
{
	if(!fDisable)
		rclContext.Execute(cmd);
}

Phobos::System::InputMapper_c::DeviceMapper_c::DeviceMapper_c(InputMapper_c &mapper, InputDevice_c &device):
	clInputDevice(device),
	rclInputMapper(mapper)
{
	clInputDevice.AddListener(*this);
}

Phobos::System::InputMapper_c::DeviceMapper_c::DeviceMapper_c(const DeviceMapper_c &rhs):
	mapActions(rhs.mapActions),
	clInputDevice(rhs.clInputDevice),
	rclInputMapper(rhs.rclInputMapper)
{
	if(rhs.hkListener.is_linked())
		clInputDevice.AddListener(*this);
}

Phobos::System::InputMapper_c::DeviceMapper_c &Phobos::System::InputMapper_c::GetDeviceMapper(const String_c &deviceName)
{
	InputDeviceMap_t::iterator	it = mapInputDevices.find(deviceName);
	if(it == mapInputDevices.end())
	{
		//device is not mapped yet, on this case binding fails
		std::stringstream stream;
		stream << "Device " << deviceName << " was not attached" << std::endl;
		PH_RAISE(INVALID_PARAMETER_EXCEPTION, "InputMapper_c::GetDeviceMapper", stream.str());
	}

	return it->second;
}

/**

	This command associates a device button with a command.

	Command syntax:

		If the command is preceed by the '+' symbol it will only be called when the
		button state is IM_InputManager_c::BUTTON_STATE_DOWN. In this case, the
		InputMapper_c will automatically call the same command preceed by '=' and
		'-' when a event IM_InputManager_c::BUTTON_STATE_UPDATE and IM_InputManager_c::BUTTON_STATE_UP
		are received, respectively.

	\param devicePathName pathName to the device
	\param actionName name of the action to associate the cmd
	\param cmd command that must be called when the action is invoked.

	\return IM_SUCCESS if no errors occurs.

*/
void Phobos::System::InputMapper_c::Bind(const String_c &devicePathName, const String_c &actionName, const String_c &cmd)
{
	this->GetDeviceMapper(devicePathName).Bind(actionName, cmd);
}

void Phobos::System::InputMapper_c::Unbind(const String_c &devicePathName, const String_c &actionName)
{
	this->GetDeviceMapper(devicePathName).Unbind(actionName);
}

void Phobos::System::InputMapper_c::InputManagerEvent(const InputManagerEvent_s &event)
{
	const String_c &deviceName = event.rclDevice.GetName();
	switch(event.eType)
	{
		case INPUT_MANAGER_EVENT_DEVICE_ATTACHED:
			{
				InputDeviceMap_t::iterator it = mapInputDevices.lower_bound(deviceName);
				if((it != mapInputDevices.end()) && !(mapInputDevices.key_comp()(deviceName, it->first)))
				{
					std::stringstream stream;
					stream << "Input device " << deviceName << " already attached." << std::endl;
					PH_RAISE(INVALID_OPERATION_EXCEPTION, "InputMapper_c::InputManagerEvent", stream.str());
				}
				else
				{
					mapInputDevices.insert(it, std::make_pair(deviceName, DeviceMapper_c(*this, event.rclDevice)));
				}
			}
			break;

		case INPUT_MANAGER_EVENT_DEVICE_DETACHED:
			mapInputDevices.erase(event.rclDevice.GetName());
			break;
	}
}

void Phobos::System::InputMapper_c::DeviceMapper_c::AddEventButtonInfo(std::stringstream &stream, const InputEvent_s &event) const
{
	stream << ' ' <<
		INPUT_EVENT_BUTTON << ' ' <<
		event.stButton.uId << ' ' <<
		event.stButton.fpPression << ' ' <<
		event.stButton.eState << ' ' <<
		clInputDevice.GetName();
}

void Phobos::System::InputMapper_c::DeviceMapper_c::OnInputEventButton(const InputEvent_s &event)
{
	ActionBindMap_t::iterator it =	mapActions.find(event.stButton.uId);

	if(it == mapActions.end())
		return;

	ActionBind_s &bind = it->second;
	std::stringstream	stream;

	switch(event.stButton.eState)
	{
		case BUTTON_STATE_DOWN:
			stream << bind.strCommand;
			this->AddEventButtonInfo(stream, event);

			rclInputMapper.Execute(stream.str());
			break;

		case BUTTON_STATE_UPDATE:
			if(bind.strCommand[0] == '+')
			{
				stream << '=' << bind.strCommand.c_str()+1;
				this->AddEventButtonInfo(stream, event);

				rclInputMapper.Execute(stream.str());
			}
			break;

		case BUTTON_STATE_UP:
			if(bind.strCommand[0] == '+')
			{
				stream << '-' << bind.strCommand.c_str()+1;
				this->AddEventButtonInfo(stream, event);

				rclInputMapper.Execute(stream.str());
			}
			break;
	}
}

void Phobos::System::InputMapper_c::DeviceMapper_c::OnInputEventThumb(const InputEvent_s &event)
{
	ActionBindMap_t::iterator it =	mapActions.find(event.stButton.uId);

	if(it == mapActions.end())
		return;

	ActionBind_s &bind = it->second;
	std::stringstream	stream;

	stream << bind.strCommand << ' ' << INPUT_EVENT_THUMB << ' ' << event.stThumb.uId << ' ' << event.stThumb.fpAxis[0] << ' ' << event.stThumb.fpAxis[1];

	rclInputMapper.Execute(stream.str());
}

/**

	This is the implementation of the console command bind.

	Its needs 3 arguments: device action cmd.
	Example: bind kb0 ESCAPE quit

	This is called by a IM_ContextManager_c command.

*/
void Phobos::System::InputMapper_c::CmdBind(const StringVector_t &args, Context_c &)
{
	//We need four parameters: bind device action cmd
	if(args.size() < 4)
	{
		Kernel_c::GetInstance().LogMessage("Usage: bind <devicePathName> <actionName> <cmd>\nExample: bind kb0 UP_ARROW forward");

		return;
	}
	else
	{
		try
		{
			this->Bind(args[1], args[2], args[3]);
		}
		catch(Exception_c &e)
		{
			Kernel_c::GetInstance().LogMessage(e.what());
		}
	}
}

void Phobos::System::InputMapper_c::CmdUnbind(const StringVector_t &args, Context_c &)
{
	//We need three parameters: bind device action
	if(args.size() < 3)
	{
		Kernel_c::GetInstance().LogMessage("Usage: unbind <devicePathName> <actionName>\nExample: unbind kb0 UP_ARROW");

		return;
	}
	else
	{
		try
		{
			this->Unbind(args[1], args[2]);
		}
		catch(Exception_c &e)
		{
			Kernel_c::GetInstance().LogMessage(e.what());
		}
	}
}

void Phobos::System::InputMapper_c::ForceButtonState(const String_c &commandName, char cmdPrefix, InputEventButtonState_e state, Float_t pressure)
{
	std::stringstream	stream;

	if(commandName[0] != cmdPrefix)
		stream << cmdPrefix;

	stream << commandName;
	stream << ' ' <<
	INPUT_EVENT_BUTTON << ' ' <<
	0 << ' ' <<
	pressure << ' ' <<
	state << ' ' <<
	"user";

	rclContext.Execute(stream.str());
}

void Phobos::System::InputMapper_c::CmdPushButton(const StringVector_t &args, Context_c &)
{
	if(args.size() < 2)
	{
		Kernel_c::GetInstance().LogMessage("Usage: pushButton <buttonName>");			
	}
	else
	{
		this->ForceButtonState(args[1], '+', BUTTON_STATE_DOWN, 1);			
	}
}

void Phobos::System::InputMapper_c::CmdReleaseButton(const StringVector_t &args, Context_c &)
{
	if(args.size() < 2)
	{
		Kernel_c::GetInstance().LogMessage("Usage: releaseButton <buttonName>");			
	}
	else
	{
		this->ForceButtonState(args[1], '-', BUTTON_STATE_UP, 0);
	}
}

Phobos::UInt_t Phobos::System::InputMapper_c::DeviceMapper_c::GetActionId(const String_c &actionName)
{
	UInt_t actionId;
	if(!clInputDevice.TryGetActionId(actionName, actionId))
	{
		std::stringstream stream;
		stream << "Device " << clInputDevice.GetName() << " does not have action " << actionName << std::endl;
		PH_RAISE(INVALID_PARAMETER_EXCEPTION, "InputMapper::DeviceMapper::GetActionId", stream.str());
	}

	return actionId;
}

void Phobos::System::InputMapper_c::DeviceMapper_c::InputEvent(const InputEvent_s &event)
{
	if(rclInputMapper.IsDisabled())
		return;

	switch(event.eType)
	{
		case INPUT_EVENT_BUTTON:
			this->OnInputEventButton(event);
			break;

		case INPUT_EVENT_CHAR:
			break;

		case INPUT_EVENT_THUMB:
			this->OnInputEventThumb(event);
			break;
	}
}

void Phobos::System::InputMapper_c::DeviceMapper_c::Bind(const String_c &action, const String_c &cmd)
{
	UInt_t actionId = this->GetActionId(action);
	mapActions[actionId] = ActionBind_s(cmd, action, actionId);
}

void Phobos::System::InputMapper_c::DeviceMapper_c::Unbind(const String_c &action)
{
	ActionBindMap_t::iterator it = mapActions.find(this->GetActionId(action));
	if(it == mapActions.end())
	{
		std::stringstream stream;
		stream << "Action " << action << " not bound to device " << clInputDevice.GetName() << std::endl;
		PH_RAISE(INVALID_PARAMETER_EXCEPTION, "InputMapper::DeviceMapper::Unbind", stream.str());
	}

	mapActions.erase(it);
}


