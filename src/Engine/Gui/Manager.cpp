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

#include "Phobos/Engine/Gui/Manager.h"

#include <Rocket/Core/SystemInterface.h>

#include "Phobos/Engine/Console.h"
#include "Phobos/Engine/Clocks.h"


#include "Phobos/Engine/Gui/Context.h"

#include <Phobos/Shell/Utils.h>
#include <Phobos/Memory.h>

#include <Phobos/System/InputManager.h>
#include <Phobos/System/InputDevice.h>
#include <Phobos/System/MouseInputDevice.h>


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
		return Phobos::Engine::GuiClock::Now().time_since_epoch().count();
	}
					
	bool SystemInterface::LogMessage(Rocket::Core::Log::Type type, const Rocket::Core::String& message)
	{
		std::stringstream stream;
		stream << "Rocket: " << message.CString();
		Phobos::LogMessage(stream.str());

		return false;
	}

	static Phobos::Engine::Gui::Manager *g_pclManager = nullptr; 
}

Phobos::Engine::Gui::Manager::LocalInputDeviceListener::LocalInputDeviceListener():
	m_pclOwner(nullptr)
{
	//empty
}
	
void Phobos::Engine::Gui::Manager::LocalInputDeviceListener::SetOwner(Phobos::Engine::Gui::Manager &owner)
{						
	m_pclOwner = &owner;
}

void Phobos::Engine::Gui::Manager::LocalInputDeviceListener::ListenTo(Phobos::System::InputDevice &device)
{
	device.AddListener(*this);
}

void Phobos::Engine::Gui::Manager::LocalInputDeviceListener::OnInputEvent(const Phobos::System::InputEvent_s &event)
{
	m_pclOwner->InputEvent(event);
}

Phobos::Engine::Gui::Manager &Phobos::Engine::Gui::Manager::GetInstance(void)
{
	PH_ASSERT(g_pclManager != nullptr);

	return *g_pclManager;
}

Phobos::Engine::Gui::Manager::Manager(const String_t &name):
	Module(name, NodeFlags::PRIVATE_CHILDREN),
	m_cmdRocketLoadFontFace("rocketLoadFontFace"),
	m_fInputActive(false)
{
	PH_ASSERT(g_pclManager == nullptr);

	g_pclManager = this;

	m_cmdRocketLoadFontFace.SetProc(PH_CONTEXT_CMD_BIND(&Phobos::Engine::Gui::Manager::CmdRocketLoadFonfFace, this));

	m_clKeyboardListener.SetOwner(*this);
	m_clMouseListener.SetOwner(*this);

	Console::GetInstance().AddContextCommand(m_cmdRocketLoadFontFace);
}

Phobos::Engine::Gui::Manager::~Manager()
{
	PH_ASSERT(g_pclManager == this);

	g_pclManager = nullptr;
}

Rocket::Core::SystemInterface *Phobos::Engine::Gui::Manager::CreateSystemInterface()
{
	return PH_NEW SystemInterface();
}

void Phobos::Engine::Gui::Manager::LoadFonts()
{
	for(auto &path : m_lstFontFacesToLoad)	
	{
		Rocket::Core::FontDatabase::LoadFontFace(path.c_str());	
	}

	m_lstFontFacesToLoad.clear();
}

Phobos::Engine::Gui::ContextPtr_t Phobos::Engine::Gui::Manager::CreateContext(const Phobos::String_t &name)
{
	ContextPtr_t ptr = Context::Create(name, this->GetScreenWidth(), this->GetScreenHeight());

	this->AddPrivateChild(*ptr);

	return ptr;
}

void Phobos::Engine::Gui::Manager::EnableInput(System::InputManager &inputManager)
{
	if(!m_fInputActive)
	{		
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

void Phobos::Engine::Gui::Manager::DisableInput(System::InputManager &inputManager)
{	
	if (m_fInputActive)
	{
		inputManager.GetDevice(System::INPUT_DEVICE_KEYBOARD).RemoveListener(m_clKeyboardListener);
		inputManager.GetDevice(System::INPUT_DEVICE_MOUSE).RemoveListener(m_clMouseListener);

		m_fInputActive = false;
	}
}

void Phobos::Engine::Gui::Manager::InputEvent(const System::InputEvent_s &event)
{	
	for(auto it : *this)	
	{
		Context *context = static_cast<Context *>(it.second);

		context->OnInputEvent(event);
	}	
}

void Phobos::Engine::Gui::Manager::OnUpdate()
{
	for(Node::const_iterator it = this->begin(), end = this->end(); it != end; ++it)
	{
		Context *context = static_cast<Context *>(it->second);

		context->Update();
	}
}

void Phobos::Engine::Gui::Manager::CmdRocketLoadFonfFace(const Shell::StringVector_t &container, Phobos::Shell::Context &)
{
	if(container.size() < 2)
	{
		LogMessage("[Phobos::Engine::Gui::Manager::CmdRocketLoadFonfFace] ERROR: insuficient parameters, usage: rocketLoadFontFace <fontName>");

		return;
	}

	m_lstFontFacesToLoad.push_back(container[1]);		
}
