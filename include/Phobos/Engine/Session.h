/*
Phobos 3d
October 2012
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

#ifndef PH_ENGINE_SESSION_H
#define PH_ENGINE_SESSION_H

#include <Phobos/System/InputDeviceListener.h>
#include <Phobos/System/InputManager.h>
#include <Phobos/System/InputMapper.h>
#include <Phobos/Singleton.h>

#include "Phobos/Engine/EngineAPI.h"

#include "Phobos/Engine/Module.h"

namespace Phobos
{	
	namespace System
	{
		class MouseInputDevice;
	}

	namespace Engine
	{
		PH_DECLARE_SINGLETON_PTR(Session);

		class IPlayerCommandProducer;
		class IClient;
		
		namespace Gui
		{
			class Form;
		}

		class PH_ENGINE_API Session: public Module,
			private System::InputManagerListener, 
			private System::InputDeviceListener
		{
			PH_DECLARE_SINGLETON_METHODS(Session);

			public:
				~Session();

				void SetPlayerCommandProducer(IPlayerCommandProducer *commandProducer);
				void SetClient(IClient *client);
				void SetGuiForm(Gui::Form *form);
				void CloseConsole();

			protected:
				virtual void OnFixedUpdate() override;

			private:
				Session();			

				virtual void OnInputManagerEvent(const System::InputManagerEvent_s &event) override;
				virtual void OnInputEvent(const System::InputEvent_s &event) override;
			
				void ClipMouseCursor();
				void UnclipMouseCursor();

				void DisableGameInput();
				void EnableGameInput();

			private:
				struct ConfigInfo_s
				{
					System::MouseInputDevice *m_pclMouse;				
				};		

				struct ConfigInfo_s GetConfig();

			private:			
				System::InputMapperPtr_t	m_ipInputMapper;
				bool						m_fIgnoreConsoleKey;

				IPlayerCommandProducer	*m_pclPlayerCommandProducer;
				IClient					*m_pclClient;
				Gui::Form				*m_pclForm;

		};
	}
}

#endif