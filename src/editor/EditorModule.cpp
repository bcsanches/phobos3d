#include "Phobos/Editor/EditorModule.h"

#include <Phobos/Exception.h>
#include <Phobos/Log.h>
#include <Phobos/Engine/Session.h>

#include <rapidjson/document.h>

#include <JsonCreator/StringWriter.h>

#include "Phobos/Editor/RequestFactory.h"
#include "Phobos/Editor/ErrorRequest.h"

PH_PLUGIN_ENTRY_POINT("EditorPluginModule", "editor.cfg")

namespace Phobos
{	
	namespace Editor
	{
		PH_DEFINE_DEFAULT_SINGLETON(EditorModule);

		PH_PLUGIN_REGISTER_MODULE(EditorModule);

		PH_PLUGIN_CREATE_MODULE_PROC_IMPL(EditorModule);				

		EditorModule::EditorModule() :
			Module("EditorModule")
		{
			this->AddChild(m_clEditObjectManager);
		}

		Phobos::Engine::EscAction EditorModule::HandleEsc(Engine::Gui::Form *&outForm)
		{
			return Engine::EscAction::IGNORE_ESC;
		}

		void EditorModule::SetPlayerCmd(Engine::IPlayerCmdPtr_t cmd)
		{
			PH_RAISE(INVALID_OPERATION_EXCEPTION, "Phobos::Editor::EditorModule::SetPlayerCmd", "Not implemented");
		}

		void EditorModule::OnBoot()
		{
			auto &session = Engine::Session::GetInstance();

			session.SetClient(this);
			session.SetGuiForm(nullptr);
			session.SetPlayerCommandProducer(nullptr);

			m_clNetworkService.Start();

			Game::WorldManager::GetInstance().AddListener(*this);
		}

		void EditorModule::OnMapUnloaded()
		{
			m_clCamera.Disable();
		}
	
		void EditorModule::OnMapLoaded()
		{
			m_clCamera.Enable();
		}

		void EditorModule::ExecuteJsonCommand(const rapidjson::Value &obj, JsonCreator::StringWriter &response)
		{
			const auto &command = obj["method"];
			if (command.IsNull())
			{
				LogMakeStream() << "[Phobos::Editor::EditorModule::OnFixedUpdate] JSON does not contains valid commands";

				ErrorRequest error(obj, "Method not specified", ErrorRequest::INVALID_REQUEST);

				error.Execute(obj, response);

				return;
			}

			auto &requestFactory = RequestFactory::GetInstance();
			if (auto request = requestFactory.TryCreate(command.GetString(), obj))
			{
				request->Execute(obj, response);
			}
			else
			{
				LogMakeStream() << "[Phobos::Editor::EditorModule::OnFixedUpdate] RPC method " << command.GetString() << " does not exists";

				std::stringstream stream;

				stream << "The method " << command.GetString() << " does not exists.";

				ErrorRequest error(obj, stream.str().c_str(), ErrorRequest::METHOD_NOT_FOUND);
				error.Execute(obj, response);
			}
		}

		void EditorModule::OnFixedUpdate()
		{
			Phobos::Shell::StringVector_t messages(m_clNetworkService.GetPendingMessages());

			if (messages.empty())
				return;

			rapidjson::Document document;

			JsonCreator::StringWriter response;

			for (const std::string &msg : messages)
			{
				document.Parse<0>(msg.c_str());

				if (document.HasParseError())
				{
					LogMakeStream() << "[Phobos::Editor::EditorModule::OnFixedUpdate] Error parsing JSON: " << msg;
					continue;
				}

				this->ExecuteJsonCommand(document, response);
			}

			if (response.GetSize() > 0)
			{
				m_clNetworkService.SendMessage(response.GetString());
			}
		}

		void EditorModule::CreateMapObject(const String_t &asset, Game::MapObjectTypes type)
		{
			auto transform = m_clCamera.GetTransform();

			Ogre::Vector3 axis[3];
			transform.GetRotation().ToAxes(axis);

			transform.SetOrigin(transform.GetOrigin() + axis[1]);

			m_clEditObjectManager.CreateEditObject(asset, type, transform);
		}

	}	//namespace Editor
}	//namespace Phobos

