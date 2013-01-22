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

#include <PH_ContextCmd.h>
#include <PH_Singleton.h>

#include "PH_CoreModule.h"
#include "PH_EngineCoreAPI.h"

#include "PH_ContextFwd.h"

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
		class InputDevice_c;
	}

	namespace Gui
	{
		PH_DECLARE_NODE_PTR(Manager);

		class PH_ENGINE_CORE_API Manager_c: public CoreModule_c
		{			
			public:
				static Manager_c &GetInstance(void);			
				static void ReleaseInstance(void);

				Gui::ContextPtr_t CreateContext(const String_c &name);

				~Manager_c();

				void EnableInput();
				void DisableInput();
								
			protected:
				Manager_c();				

				virtual void OnPrepareToBoot();				
				virtual void OnFixedUpdate();
				virtual void OnUpdate();				

				virtual size_t GetScreenWidth() = 0;
				virtual size_t GetScreenHeight() = 0;

				Rocket::Core::SystemInterface *CreateSystemInterface();

				void LoadFonts();

				void InputEvent(const System::InputEvent_s &event);

			protected:
				static void UpdateInstance(ManagerPtr_t manager);
			
			private:								
				void CmdRocketLoadFonfFace(const StringVector_t &container, Phobos::Context_c &);

			private:
				class LocalInputDeviceListener_c: public System::InputDeviceListener_c
				{
					public:
						LocalInputDeviceListener_c();

						void SetOwner(Manager_c &manager);

						void ListenTo(System::InputDevice_c &device);

						void InputEvent(const System::InputEvent_s &event);

					private:
						Manager_c *pclOwner;
				};

				LocalInputDeviceListener_c clKeyboardListener;
				LocalInputDeviceListener_c clMouseListener;

				std::list<String_c> lstFontFacesToLoad;

				bool				fInputActive;
				bool				fDisableInput;

				ContextCmd_c		cmdRocketLoadFontFace;				
		};
	}
}

#endif
