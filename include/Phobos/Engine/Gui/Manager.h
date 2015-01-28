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

#ifndef PH_GUI_MANAGER_H
#define PH_GUI_MANAGER_H

#include <list>

#include <Phobos/System/InputDeviceListener.h>

#include <Phobos/Shell/Command.h>
#include <Phobos/Singleton.h>
#include <Phobos/System/InputManagerFwd.h>

#include "Phobos/Engine/Module.h"
#include "Phobos/Engine/EngineAPI.h"

#include "Phobos/Engine/Gui/ContextFwd.h"

namespace Rocket
{
	namespace Core
	{
		class SystemInterface;
	}
}

namespace Phobos
{
	namespace System
	{
		class InputDevice;
	}

	namespace Engine
	{
		class Console;

		namespace Gui
		{
			PH_DECLARE_NODE_PTR(Manager);

			class PH_ENGINE_API Manager: public Module
			{			
				public:
					static Manager &GetInstance(void);					

					Gui::ContextPtr_t CreateContext(const String_t &name);

					virtual ~Manager();

					void EnableInput(System::InputManager &inputManager);
					void DisableInput(System::InputManager &inputManager);
								
				protected:
					Manager(const String_t &name);
			
					virtual void OnUpdate() override;				

					virtual size_t GetScreenWidth() = 0;
					virtual size_t GetScreenHeight() = 0;

					Rocket::Core::SystemInterface *CreateSystemInterface();

					void LoadFonts();

					void InputEvent(const System::InputEvent_s &event);				
			
				private:								
					void CmdRocketLoadFonfFace(const Shell::StringVector_t &container, Phobos::Shell::Context &);

				private:
					class LocalInputDeviceListener: public System::InputDeviceListener
					{
						public:
							LocalInputDeviceListener();

							void SetOwner(Manager &manager);

							void ListenTo(System::InputDevice &device);

							virtual void OnInputEvent(const System::InputEvent_s &event) override;

						private:
							Manager *m_pclOwner;
					};

					LocalInputDeviceListener m_clKeyboardListener;
					LocalInputDeviceListener m_clMouseListener;

					std::list<String_t> m_lstFontFacesToLoad;

					bool				m_fInputActive;					

					Shell::Command		m_cmdRocketLoadFontFace;				
			};
		}
	}
}

#endif
