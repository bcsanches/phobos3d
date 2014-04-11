#include "Phobos/Editor/EditorClient.h"

#include "Phobos/Engine/Session.h"

namespace Phobos
{
	namespace Editor
	{
		void EditorClient::DispatchCommand(Engine::IPlayerCmdPtr_t cmd)
		{
			//empty
		}

		Engine::EscAction EditorClient::HandleEsc(Engine::Gui::Form *&outForm)
		{
			return Engine::EscAction::IGNORE_ESC;
		}

		void EditorClient::OnConnect()
		{
			auto &session = Engine::Session::GetInstance();

			session.SetGuiForm(nullptr);
			session.SetPlayerCommandProducer(nullptr);
		}
	}
}
