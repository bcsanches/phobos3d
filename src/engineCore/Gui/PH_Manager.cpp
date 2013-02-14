/*
Phobos 3d
September 2012
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

#include "Gui/PH_Manager.h"

#include <Rocket/Core/SystemInterface.h>

#include <PH_Console.h>
#include <PH_Core.h>

#include <Phobos/Shell/Utils.h>
#include <Phobos/Memory.h>

#include <Phobos/System/InputManager.h>
#include <Phobos/System/InputDevice.h>
#include <Phobos/System/MouseInputDevice.h>

#include "Gui/PH_Context.h"

namespace 
{
	class SystemInterface: public Rocket::Core::SystemInterface
	{
		public:
			virtual float GetElapsedTime();

			/// Logs the specified message.
			virtual bool LogMessage(Rocket::Core::Log::Type type, const Rocket::Core::String& message);
	};

	float SystemInterface::GetElapsedTime()
	{
		return Phobos::Core::GetInstance().GetUiTimer().m_fpTotalTicks;
	}
					
	bool SystemInterface::LogMessage(Rocket::Core::Log::Type type, const Rocket::Core::String& message)
	{
		std::stringstream stream;
		stream << "Rocket: " << message.CString();
		Phobos::LogMessage(stream.str());

		return false;
	}

	static Phobos::Gui::ManagerPtr_t ipInstance_gl;
}

Phobos::Gui::Manager::LocalInputDeviceListener::LocalInputDeviceListener():
	m_pclOwner(NULL)
{
	//empty
}
	
void Phobos::Gui::Manager::LocalInputDeviceListener::SetOwner(Phobos::Gui::Manager &owner)
{						
	m_pclOwner = &owner;
}

void Phobos::Gui::Manager::LocalInputDeviceListener::ListenTo(Phobos::System::InputDevice &device)
{
	device.AddListener(*this);
}

void Phobos::Gui::Manager::LocalInputDeviceListener::OnInputEvent(const Phobos::System::InputEvent_s &event)
{
	m_pclOwner->InputEvent(event);
}

Phobos::Gui::Manager &Phobos::Gui::Manager::GetInstance(void)
{
	return *ipInstance_gl;
}

void Phobos::Gui::Manager::ReleaseInstance(void)
{
	ipInstance_gl.reset();
}

void Phobos::Gui::Manager::UpdateInstance(ManagerPtr_t manager)
{
	ipInstance_gl = manager;
}

Phobos::Gui::Manager::Manager():
	CoreModule("GuiManager", NodeFlags::PRIVATE_CHILDREN),
	m_cmdRocketLoadFontFace("rocketLoadFontFace"),
	m_fInputActive(false),
	m_fDisableInput(false)
{
	m_cmdRocketLoadFontFace.SetProc(PH_CONTEXT_CMD_BIND(&Phobos::Gui::Manager::CmdRocketLoadFonfFace, this));

	m_clKeyboardListener.SetOwner(*this);
	m_clMouseListener.SetOwner(*this);
}

Phobos::Gui::Manager::~Manager()
{
	//empty
}

void Phobos::Gui::Manager::OnPrepareToBoot()
{
	Console &console = Console::GetInstance();

	console.AddContextCommand(m_cmdRocketLoadFontFace);
}


Rocket::Core::SystemInterface *Phobos::Gui::Manager::CreateSystemInterface()
{
	return PH_NEW SystemInterface();
}

void Phobos::Gui::Manager::LoadFonts()
{
	for(auto &path : m_lstFontFacesToLoad)	
	{
		Rocket::Core::FontDatabase::LoadFontFace(path.c_str());	
	}

	m_lstFontFacesToLoad.clear();
}

Phobos::Gui::ContextPtr_t Phobos::Gui::Manager::CreateContext(const Phobos::String_t &name)
{
	ContextPtr_t ptr = Context::Create(name, this->GetScreenWidth(), this->GetScreenHeight());

	this->AddPrivateChild(*ptr);

	return ptr;
}

void Phobos::Gui::Manager::EnableInput()
{
	if(!m_fInputActive)
	{
		auto &inputManager = System::InputManager::GetInstance();

		auto &mouse = static_cast<System::MouseInputDevice &>(inputManager.GetDevice(System::INPUT_DEVICE_MOUSE));

		m_clKeyboardListener.ListenTo(inputManager.GetDevice(System::INPUT_DEVICE_KEYBOARD));	
		m_clMouseListener.ListenTo(mouse);	

		m_fInputActive = true;

		int x = mouse.GetX();
		int y = mouse.GetY();
		
		for(Node::const_iterator it = this->begin(), end = this->end(); it != end; ++it)
		{
			Context *context = static_cast<Context *>(it->second);

			context->ResetMousePosition(x, y);
		}
	}
}

void Phobos::Gui::Manager::DisableInput()
{
	//we must delay input deactivation to avoid causing problem on listener lists
	m_fDisableInput = true;	
}

void Phobos::Gui::Manager::InputEvent(const System::InputEvent_s &event)
{	
	for(auto it : *this)	
	{
		Context *context = static_cast<Context *>(it.second);

		context->OnInputEvent(event);
	}	
}

void Phobos::Gui::Manager::OnFixedUpdate()
{
	if(m_fInputActive && m_fDisableInput)
	{
		auto &inputManager = System::InputManager::GetInstance();

		inputManager.GetDevice(System::INPUT_DEVICE_KEYBOARD).RemoveListener(m_clKeyboardListener);
		inputManager.GetDevice(System::INPUT_DEVICE_MOUSE).RemoveListener(m_clMouseListener);

		m_fInputActive = m_fDisableInput = false;
	}
}

void Phobos::Gui::Manager::OnUpdate()
{
	for(Node::const_iterator it = this->begin(), end = this->end(); it != end; ++it)
	{
		Context *context = static_cast<Context *>(it->second);

		context->Update();
	}
}

void Phobos::Gui::Manager::CmdRocketLoadFonfFace(const Shell::StringVector_t &container, Phobos::Shell::Context &)
{
	if(container.size() < 2)
	{
		LogMessage("[Phobos::Gui::Manager::CmdRocketLoadFonfFace] ERROR: insuficient parameters, usage: rocketLoadFontFace <fontName>");

		return;
	}

	m_lstFontFacesToLoad.push_back(container[1]);		
}
