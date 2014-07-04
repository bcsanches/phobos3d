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

#ifndef PH_SYSTEM_INPUT_MAPPER_H
#define PH_SYSTEM_INPUT_MAPPER_H

#include <Phobos/Shell/IContext.h>
#include <Phobos/Shell/Command.h>
#include <Phobos/Node.h>

#include "Phobos/System/InputDeviceFwd.h"
#include "Phobos/System/InputManager.h"
#include "Phobos/System/InputMapperFwd.h"

namespace Phobos
{	
	namespace System
	{		
		class PH_SYSTEM_API InputMapper: public Node, private InputManagerListener
		{		
			private:
				// =====================================================
				// PRIVATE TYPES
				// =====================================================
				struct ActionBind_s
				{
					inline ActionBind_s(const String_t &command, const String_t &actionName, UInt_t action):
						m_strCommand(command),
						m_strActionName(actionName),
						m_uDeviceAction(action)
					{
						//empty
					}

					String_t	m_strCommand;
					String_t	m_strActionName;			
					UInt_t		m_uDeviceAction;
				};

				typedef std::map<UInt_t, ActionBind_s> ActionBindMap_t;
				typedef ActionBindMap_t::value_type ActionBindMapPair_t;			

				class DeviceMapper: private InputDeviceListener
				{
					public:
						DeviceMapper(InputMapper &mapper, InputDevice &device);
						DeviceMapper(const DeviceMapper &rhs);

						DeviceMapper &operator =(const DeviceMapper &rhs);

						void Bind(const String_t &actionName, const String_t &cmd);
						void Unbind(const String_t &actionName);

					private:
						virtual void OnInputEvent(const InputEvent_s &event) override;						

						void OnInputEventButton(const InputEvent_s &event);
						void OnInputEventThumb(const InputEvent_s &event);
						void AddEventButtonInfo(std::stringstream &stream, const InputEvent_s &event) const;

						UInt_t GetActionId(const String_t &name);					

					private:
						ActionBindMap_t				m_mapActions;

						InputDevice					&m_clInputDevice;
						InputMapper					&m_rclInputMapper;				
				};				

			public:
				// =====================================================
				// PUBLIC METHODS
				// =====================================================				
				InputMapper(const String_t &name, Shell::IContext &context, InputManager &inputManager);
				~InputMapper(void);

				void Bind(const String_t &devicePathName, const String_t &actionName, const String_t &cmd);
				void Unbind(const String_t &devicePathName, const String_t &actionName);

				inline void Disable();
				inline void Enable();
				inline bool IsDisabled() const;

				void Execute(const std::string &cmd);

			private:
				// =====================================================
				// PRIVATE METHODS
				// =====================================================			
				void OnDeviceAttached(InputDevicePtr_t &device);
				void OnDeviceDetached(InputDevicePtr_t &device);

				void AddEventButtonInfo(std::stringstream &stream, InputEvent_s &event, DeviceMapper &info) const;

				void OnInputEventButton(InputEvent_s &event, DeviceMapper &info);		
				void OnInputEventThumb(InputEvent_s &event, DeviceMapper &info);
			
				DeviceMapper &GetDeviceMapper(const String_t &deviceName);

				virtual void OnInputManagerEvent(const InputManagerEvent_s &event) override;

				void MapDevice(InputDevice &device);

				void ForceButtonState(const String_t &commandName, char cmdPrefix, InputEventButtonState_e state, Float_t pressure);

				void CmdUnbind(const Shell::StringVector_t &args, Shell::Context &);
				void CmdBind(const Shell::StringVector_t &args, Shell::Context &);
				void CmdPushButton(const Shell::StringVector_t &args, Shell::Context &);
				void CmdReleaseButton(const Shell::StringVector_t &args, Shell::Context &);

			private:
				// =====================================================
				// PRIVATE ATRIBUTES
				// =====================================================			
				typedef std::map<String_t, DeviceMapper> InputDeviceMap_t;
				InputDeviceMap_t		m_mapInputDevices;

				Shell::IContext			&m_rclContext;

				bool					m_fDisable;				

				Shell::Command			m_cmdBind;
				Shell::Command			m_cmdUnbind;
				Shell::Command			m_cmdPushButton;
				Shell::Command			m_cmdReleaseButton;

				InputManager			&m_rclInputManager;
		};

		inline void InputMapper::Disable()
		{
			m_fDisable = true;
		}

		inline void InputMapper::Enable()
		{
			m_fDisable = false;
		}

		inline bool InputMapper::IsDisabled() const
		{
			return(m_fDisable);
		}
	}
}

#endif
