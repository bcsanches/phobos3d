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

#include <Phobos/Shell/Utils.h>

#include <Phobos/Exception.h>
#include <Phobos/Log.h>
#include <Phobos/Memory.h>
#include <Phobos/Path.h>

#include "Phobos/System/InputDevice.h"
#include "Phobos/System/InputEvent.h"
#include "Phobos/System/InputManager.h"

Phobos::System::InputMapperPtr_t Phobos::System::InputMapper::Create(const String_t &name, Shell::IContext &context, InputManager &inputManager)
{
	return std::make_shared<InputMapper>(name, context, inputManager);	
}

Phobos::System::InputMapper::InputMapper(const String_t &name, Shell::IContext &context, InputManager &inputManager):
	Node(name),
	m_rclContext(context),
	m_rclInputManager(inputManager),
	m_fDisable(false),
	m_cmdBind("bind"),
	m_cmdUnbind("unbind"),
	m_cmdPushButton("pushButton"),
	m_cmdReleaseButton("releaseButton")
{	
	m_rclInputManager.AddListener(*this);

	m_cmdBind.SetProc(PH_CONTEXT_CMD_BIND(&InputMapper::CmdBind, this));
	m_cmdUnbind.SetProc(PH_CONTEXT_CMD_BIND(&InputMapper::CmdUnbind, this));
	m_cmdPushButton.SetProc(PH_CONTEXT_CMD_BIND(&InputMapper::CmdPushButton, this));
	m_cmdReleaseButton.SetProc(PH_CONTEXT_CMD_BIND(&InputMapper::CmdReleaseButton, this));

	m_rclContext.AddContextCommand(m_cmdBind);
	m_rclContext.AddContextCommand(m_cmdUnbind);
	m_rclContext.AddContextCommand(m_cmdPushButton);
	m_rclContext.AddContextCommand(m_cmdReleaseButton);

	m_rclInputManager.Accept(std::bind(&InputMapper::MapDevice, this, std::placeholders::_1));
}

Phobos::System::InputMapper::~InputMapper(void)
{
	//empty
}

void Phobos::System::InputMapper::Execute(const std::string &cmd)
{
	if(!m_fDisable)
		m_rclContext.Execute(cmd);
}

Phobos::System::InputMapper::DeviceMapper::DeviceMapper(InputMapper &mapper, InputDevice &device):
	m_clInputDevice(device),
	m_rclInputMapper(mapper)
{
	m_clInputDevice.AddListener(*this);
}

Phobos::System::InputMapper::DeviceMapper::DeviceMapper(const DeviceMapper &rhs):
	m_mapActions(rhs.m_mapActions),
	m_clInputDevice(rhs.m_clInputDevice),
	m_rclInputMapper(rhs.m_rclInputMapper)
{
	if(rhs.m_hkListener.is_linked())
		m_clInputDevice.AddListener(*this);
}

Phobos::System::InputMapper::DeviceMapper &Phobos::System::InputMapper::GetDeviceMapper(const String_t &deviceName)
{
	InputDeviceMap_t::iterator	it = m_mapInputDevices.find(deviceName);
	if(it == m_mapInputDevices.end())
	{
		//device is not mapped yet, on this case binding fails
		std::stringstream stream;
		stream << "Device " << deviceName << " was not attached" << std::endl;
		PH_RAISE(INVALID_PARAMETER_EXCEPTION, "InputMapper::GetDeviceMapper", stream.str());
	}

	return it->second;
}

/**

	This command associates a device button with a command.

	Command syntax:

		If the command is preceed by the '+' symbol it will only be called when the
		button state is IM_InputManager_c::BUTTON_STATE_DOWN. In this case, the
		InputMapper will automatically call the same command preceed by '=' and
		'-' when a event IM_InputManager_c::BUTTON_STATE_UPDATE and IM_InputManager_c::BUTTON_STATE_UP
		are received, respectively.

	\param devicePathName pathName to the device
	\param actionName name of the action to associate the cmd
	\param cmd command that must be called when the action is invoked.

	\return IM_SUCCESS if no errors occurs.

*/
void Phobos::System::InputMapper::Bind(const String_t &devicePathName, const String_t &actionName, const String_t &cmd)
{
	this->GetDeviceMapper(devicePathName).Bind(actionName, cmd);
}

void Phobos::System::InputMapper::Unbind(const String_t &devicePathName, const String_t &actionName)
{
	this->GetDeviceMapper(devicePathName).Unbind(actionName);
}

void Phobos::System::InputMapper::MapDevice(Phobos::System::InputDevice &device)
{
	auto &deviceName = device.GetName();

	InputDeviceMap_t::iterator it = m_mapInputDevices.lower_bound(deviceName);
	if ((it != m_mapInputDevices.end()) && !(m_mapInputDevices.key_comp()(deviceName, it->first)))
	{
		std::stringstream stream;
		stream << "Input device " << deviceName << " already attached." << std::endl;
		PH_RAISE(INVALID_OPERATION_EXCEPTION, "InputMapper::InputManagerEvent", stream.str());
	}
	else
	{
		m_mapInputDevices.insert(it, std::make_pair(deviceName, DeviceMapper(*this, device)));
	}
}

void Phobos::System::InputMapper::OnInputManagerEvent(const InputManagerEvent_s &event)
{
	const String_t &deviceName = event.m_rclDevice.GetName();
	switch(event.m_eType)
	{
		case INPUT_MANAGER_EVENT_DEVICE_ATTACHED:
			{
				this->MapDevice(event.m_rclDevice);
			}
			break;

		case INPUT_MANAGER_EVENT_DEVICE_DETACHED:
			m_mapInputDevices.erase(event.m_rclDevice.GetName());
			break;
	}
}

void Phobos::System::InputMapper::DeviceMapper::AddEventButtonInfo(std::stringstream &stream, const InputEvent_s &event) const
{
	stream << ' ' <<
		INPUT_EVENT_BUTTON << ' ' <<
		event.m_stButton.m_uId << ' ' <<
		event.m_stButton.m_fpPression << ' ' <<
		event.m_stButton.m_eState << ' ' <<
		m_clInputDevice.GetName();
}

void Phobos::System::InputMapper::DeviceMapper::OnInputEventButton(const InputEvent_s &event)
{
	ActionBindMap_t::iterator it =	m_mapActions.find(event.m_stButton.m_uId);

	if(it == m_mapActions.end())
		return;

	ActionBind_s &bind = it->second;
	std::stringstream	stream;

	switch(event.m_stButton.m_eState)
	{
		case BUTTON_STATE_DOWN:
			stream << bind.m_strCommand;
			this->AddEventButtonInfo(stream, event);

			m_rclInputMapper.Execute(stream.str());
			break;

		case BUTTON_STATE_UPDATE:
			if(bind.m_strCommand[0] == '+')
			{
				stream << '=' << bind.m_strCommand.c_str()+1;
				this->AddEventButtonInfo(stream, event);

				m_rclInputMapper.Execute(stream.str());
			}
			break;

		case BUTTON_STATE_UP:
			if(bind.m_strCommand[0] == '+')
			{
				stream << '-' << bind.m_strCommand.c_str()+1;
				this->AddEventButtonInfo(stream, event);

				m_rclInputMapper.Execute(stream.str());
			}
			break;
	}
}

void Phobos::System::InputMapper::DeviceMapper::OnInputEventThumb(const InputEvent_s &event)
{
	ActionBindMap_t::iterator it =	m_mapActions.find(event.m_stButton.m_uId);

	if(it == m_mapActions.end())
		return;

	ActionBind_s &bind = it->second;
	std::stringstream	stream;

	stream << bind.m_strCommand << ' ' << INPUT_EVENT_THUMB << ' ' << event.m_stThumb.m_uId << ' ' << event.m_stThumb.m_fpAxis[0] << ' ' << event.m_stThumb.m_fpAxis[1];

	m_rclInputMapper.Execute(stream.str());
}

/**

	This is the implementation of the console command bind.

	Its needs 3 arguments: device action cmd.
	Example: bind kb0 ESCAPE quit

	This is called by a IM_ContextManager_c command.

*/
void Phobos::System::InputMapper::CmdBind(const Shell::StringVector_t &args, Shell::Context &)
{
	//We need four parameters: bind device action cmd
	if(args.size() < 4)
	{
		Phobos::LogMessage("Usage: bind <devicePathName> <actionName> <cmd>\nExample: bind kb0 UP_ARROW forward");

		return;
	}
	else
	{
		try
		{
			this->Bind(args[1], args[2], args[3]);
		}
		catch(Exception &e)
		{
			LogMessage(e.what());
		}
	}
}

void Phobos::System::InputMapper::CmdUnbind(const Shell::StringVector_t &args, Shell::Context &)
{
	//We need three parameters: bind device action
	if(args.size() < 3)
	{
		LogMessage("Usage: unbind <devicePathName> <actionName>\nExample: unbind kb0 UP_ARROW");

		return;
	}
	else
	{
		try
		{
			this->Unbind(args[1], args[2]);
		}
		catch(Exception &e)
		{
			LogMessage(e.what());
		}
	}
}

void Phobos::System::InputMapper::ForceButtonState(const String_t &commandName, char cmdPrefix, InputEventButtonState_e state, Float_t pressure)
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

	m_rclContext.Execute(stream.str());
}

void Phobos::System::InputMapper::CmdPushButton(const Shell::StringVector_t &args, Shell::Context &)
{
	if(args.size() < 2)
	{
		LogMessage("Usage: pushButton <buttonName>");			
	}
	else
	{
		this->ForceButtonState(args[1], '+', BUTTON_STATE_DOWN, 1);			
	}
}

void Phobos::System::InputMapper::CmdReleaseButton(const Shell::StringVector_t &args, Shell::Context &)
{
	if(args.size() < 2)
	{
		LogMessage("Usage: releaseButton <buttonName>");			
	}
	else
	{
		this->ForceButtonState(args[1], '-', BUTTON_STATE_UP, 0);
	}
}

Phobos::UInt_t Phobos::System::InputMapper::DeviceMapper::GetActionId(const String_t &actionName)
{
	UInt_t actionId;
	if(!m_clInputDevice.TryGetActionId(actionName, actionId))
	{
		std::stringstream stream;
		stream << "Device " << m_clInputDevice.GetName() << " does not have action " << actionName << std::endl;
		PH_RAISE(INVALID_PARAMETER_EXCEPTION, "InputMapper::DeviceMapper::GetActionId", stream.str());
	}

	return actionId;
}

void Phobos::System::InputMapper::DeviceMapper::OnInputEvent(const InputEvent_s &event)
{
	if(m_rclInputMapper.IsDisabled())
		return;

	switch(event.m_eType)
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

void Phobos::System::InputMapper::DeviceMapper::Bind(const String_t &action, const String_t &cmd)
{
	UInt_t actionId = this->GetActionId(action);
	m_mapActions.insert(std::make_pair(actionId, ActionBind_s(cmd, action, actionId)));	
}

void Phobos::System::InputMapper::DeviceMapper::Unbind(const String_t &action)
{
	ActionBindMap_t::iterator it = m_mapActions.find(this->GetActionId(action));
	if(it == m_mapActions.end())
	{
		std::stringstream stream;
		stream << "Action " << action << " not bound to device " << m_clInputDevice.GetName() << std::endl;
		PH_RAISE(INVALID_PARAMETER_EXCEPTION, "InputMapper::DeviceMapper::Unbind", stream.str());
	}

	m_mapActions.erase(it);
}


