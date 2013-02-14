#include "Editor/PH_EditorModule.h"

#include <Phobos/Exception.h>
#include <Phobos/Log.h>
#include <PH_Session.h>

#include <rapidjson/document.h>

#include <JsonCreator/StringWriter.h>

#include "Editor/PH_RequestFactory.h"

PH_GAME_PLUGIN_ENTRY_POINT("EditorPluginModule", "editor.cfg")

namespace Phobos
{	
	namespace Editor
	{
		PH_DEFINE_DEFAULT_SINGLETON(EditorModule);

		PH_GAME_PLUGIN_REGISTER_MODULE(EditorModule);

		PH_GAME_PLUGIN_CREATE_MODULE_PROC_IMPL(EditorModule);				
	}
}

Phobos::Editor::EditorModule::EditorModule():
	CoreModule("EditorModule")
{
	//empty
}

Phobos::EscAction Phobos::Editor::EditorModule::HandleEsc(Gui::Form *&outForm)
{
	return EscAction::IGNORE_ESC;
}


void Phobos::Editor::EditorModule::SetPlayerCmd(IPlayerCmdPtr_t cmd)
{
	PH_RAISE(INVALID_OPERATION_EXCEPTION, "Phobos::Editor::EditorModule::SetPlayerCmd", "Not implemented");
}

void Phobos::Editor::EditorModule::OnBoot()
{
	Session &session = Session::GetInstance();

	session.SetClient(this);
	session.SetGuiForm(nullptr);
	session.SetPlayerCommandProducer(nullptr);

	m_clNetworkService.Start();
}

void Phobos::Editor::EditorModule::ExecuteJsonCommand(const rapidjson::Value &obj, JsonCreator::StringWriter &response)
{		
	const auto &command = obj["command"];
	if(command.IsNull())
	{		
		LogMakeStream() << "[Phobos::Editor::EditorModule::OnFixedUpdate] JSON does not contains valid commands";
		return;
	}

	auto &requestFactory = RequestFactory::GetInstance();
	auto request = requestFactory.Create(command.GetString(), obj);
	request->Execute(response);
}

void Phobos::Editor::EditorModule::OnFixedUpdate()
{
	Phobos::Shell::StringVector_t messages(m_clNetworkService.GetPendingMessages());

	if(messages.empty())
		return;	

	rapidjson::Document document;

	JsonCreator::StringWriter response;

	for(const std::string &msg : messages)
	{
		document.Parse<0>(msg.c_str());

		if(document.HasParseError())		
		{
			LogMakeStream() << "[Phobos::Editor::EditorModule::OnFixedUpdate] Error parsing JSON: " << msg;
			continue;
		}

		const auto &commandList = document["commands"];
		if(!commandList.IsNull())
		{
			if(!commandList.IsArray())
			{
				LogMakeStream() << "[Phobos::Editor::EditorModule::OnFixedUpdate] Command list is no an array: " << msg;
				continue;
			}

			for (rapidjson::Value::ConstValueIterator itr = commandList.Begin(); itr != commandList.End(); ++itr)
				this->ExecuteJsonCommand(*itr, response);
		}
		else
		{			
			this->ExecuteJsonCommand(document, response);
		}		
	}

	if(response.GetSize() > 0)
	{
		m_clNetworkService.SendMessage(response.GetString());
	}
}

