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
#include <PH_ContextUtils.h>
#include <PH_Core.h>
#include <PH_Kernel.h>
#include <PH_Memory.h>

#include <Phobos/System/InputManager.h>
#include <Phobos/System/InputDevice.h>
#include <Phobos/System/MouseInputDevice.h>

#include "Gui/PH_Context.h"

namespace 
{
	class SystemInterface_c: public Rocket::Core::SystemInterface
	{
		public:
			virtual float GetElapsedTime();

			/// Logs the specified message.
			virtual bool LogMessage(Rocket::Core::Log::Type type, const Rocket::Core::String& message);
	};

	float SystemInterface_c::GetElapsedTime()
	{
		return Phobos::Core_c::GetInstance().GetUiTimer().fpTotalTicks;
	}
					
	bool SystemInterface_c::LogMessage(Rocket::Core::Log::Type type, const Rocket::Core::String& message)
	{
		std::stringstream stream;
		stream << "Rocket: " << message.CString();
		Phobos::Kernel_c::GetInstance().LogMessage(stream.str());

		return false;
	}

	static Phobos::Gui::ManagerPtr_t ipInstance_gl;
}

Phobos::Gui::Manager_c::LocalInputDeviceListener_c::LocalInputDeviceListener_c():
	pclOwner(NULL)
{
	//empty
}
	
void Phobos::Gui::Manager_c::LocalInputDeviceListener_c::SetOwner(Phobos::Gui::Manager_c &owner)
{						
	pclOwner = &owner;
}

void Phobos::Gui::Manager_c::LocalInputDeviceListener_c::ListenTo(Phobos::System::InputDevice_c &device)
{
	device.AddListener(*this);
}

void Phobos::Gui::Manager_c::LocalInputDeviceListener_c::InputEvent(const Phobos::System::InputEvent_s &event)
{
	pclOwner->InputEvent(event);
}

Phobos::Gui::Manager_c &Phobos::Gui::Manager_c::GetInstance(void)
{
	return *ipInstance_gl;
}

void Phobos::Gui::Manager_c::ReleaseInstance(void)
{
	ipInstance_gl.reset();
}

void Phobos::Gui::Manager_c::UpdateInstance(ManagerPtr_t manager)
{
	ipInstance_gl = manager;
}

Phobos::Gui::Manager_c::Manager_c():
	CoreModule_c("GuiManager", NodeFlags::PRIVATE_CHILDREN),
	cmdRocketLoadFontFace("rocketLoadFontFace"),
	fInputActive(false),
	fDisableInput(false)
{
	cmdRocketLoadFontFace.SetProc(PH_CONTEXT_CMD_BIND(&Phobos::Gui::Manager_c::CmdRocketLoadFonfFace, this));

	clKeyboardListener.SetOwner(*this);
	clMouseListener.SetOwner(*this);
}

Phobos::Gui::Manager_c::~Manager_c()
{
	//empty
}

void Phobos::Gui::Manager_c::OnPrepareToBoot()
{
	Console_c &console = Console_c::GetInstance();

	console.AddContextCmd(cmdRocketLoadFontFace);
}


Rocket::Core::SystemInterface *Phobos::Gui::Manager_c::CreateSystemInterface()
{
	return PH_NEW SystemInterface_c();
}

void Phobos::Gui::Manager_c::LoadFonts()
{
	for(auto &path : lstFontFacesToLoad)	
	{
		Rocket::Core::FontDatabase::LoadFontFace(path.c_str());	
	}

	lstFontFacesToLoad.clear();
}

Phobos::Gui::ContextPtr_t Phobos::Gui::Manager_c::CreateContext(const Phobos::String_t &name)
{
	ContextPtr_t ptr = Context_c::Create(name, this->GetScreenWidth(), this->GetScreenHeight());

	this->AddPrivateChild(*ptr);

	return ptr;
}

void Phobos::Gui::Manager_c::EnableInput()
{
	if(!fInputActive)
	{
		auto &inputManager = System::InputManager_c::GetInstance();

		auto &mouse = static_cast<System::MouseInputDevice_c &>(inputManager.GetDevice(System::INPUT_DEVICE_MOUSE));

		clKeyboardListener.ListenTo(inputManager.GetDevice(System::INPUT_DEVICE_KEYBOARD));	
		clMouseListener.ListenTo(mouse);	

		fInputActive = true;

		int x = mouse.GetX();
		int y = mouse.GetY();
		
		for(Node_c::const_iterator it = this->begin(), end = this->end(); it != end; ++it)
		{
			Context_c *context = static_cast<Context_c *>(it->second);

			context->ResetMousePosition(x, y);
		}
	}
}

void Phobos::Gui::Manager_c::DisableInput()
{
	//we must delay input deactivation to avoid causing problem on listener lists
	fDisableInput = true;	
}

void Phobos::Gui::Manager_c::InputEvent(const System::InputEvent_s &event)
{	
	for(Node_c::const_iterator it = this->begin(), end = this->end(); it != end; ++it)
	{
		Context_c *context = static_cast<Context_c *>(it->second);

		context->InputEvent(event);
	}	
}

void Phobos::Gui::Manager_c::OnFixedUpdate()
{
	if(fInputActive && fDisableInput)
	{
		auto &inputManager = System::InputManager_c::GetInstance();

		inputManager.GetDevice(System::INPUT_DEVICE_KEYBOARD).RemoveListener(clKeyboardListener);
		inputManager.GetDevice(System::INPUT_DEVICE_MOUSE).RemoveListener(clMouseListener);

		fInputActive = fDisableInput = false;
	}
}

void Phobos::Gui::Manager_c::OnUpdate()
{
	for(Node_c::const_iterator it = this->begin(), end = this->end(); it != end; ++it)
	{
		Context_c *context = static_cast<Context_c *>(it->second);

		context->Update();
	}
}

void Phobos::Gui::Manager_c::CmdRocketLoadFonfFace(const StringVector_t &container, Phobos::Context_c &)
{
	if(container.size() < 2)
	{
		Kernel_c::GetInstance().LogMessage("[Phobos::Gui::Manager_c::CmdRocketLoadFonfFace] ERROR: insuficient parameters, usage: rocketLoadFontFace <fontName>");

		return;
	}

	lstFontFacesToLoad.push_back(container[1]);		
}
