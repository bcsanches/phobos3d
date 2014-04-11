#ifndef PH_PHOBOS_EDITOR_CLIENT_H
#define PH_PHOBOS_EDITOR_CLIENT_H

#include "Phobos/Engine/Client.h"

namespace Phobos
{
	namespace Editor
	{
		class EditorClient : public Engine::Client
		{
			public:	
				virtual void DispatchCommand(Engine::IPlayerCmdPtr_t cmd) override;

				Engine::EscAction HandleEsc(Engine::Gui::Form *&outForm) override;

			protected:
				virtual void OnConnect() override;
		};
	}
}

#endif