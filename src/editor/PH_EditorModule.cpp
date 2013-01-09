#include "Editor/PH_EditorModule.h"

#include <PH_Exception.h>
#include <PH_Kernel.h>
#include <PH_Session.h>

#include <rapidjson/document.h>

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

Phobos::Editor::EditorModule_c::EditorModule_c():
	CoreModule_c("EditorModule")
{
	//empty
}

Phobos::EscAction::Enum Phobos::Editor::EditorModule_c::HandleEsc(Gui::Form_c *&outForm)
{
	return EscAction::IGNORE_ESC;
}


void Phobos::Editor::EditorModule_c::SetPlayerCmd(IPlayerCmdPtr_t cmd)
{
	PH_RAISE(INVALID_OPERATION_EXCEPTION, "Phobos::Editor::EditorModule_c::SetPlayerCmd", "Not implemented");
}

void Phobos::Editor::EditorModule_c::OnBoot()
{
	Session_c &session = Session_c::GetInstance();

	session.SetClient(this);
	session.SetGuiForm(nullptr);
	session.SetPlayerCommandProducer(nullptr);

	clNetworkService.Start();
}

void Phobos::Editor::EditorModule_c::ExecuteJsonCommand(const rapidjson::Value &obj)
{		
	const auto &command = obj["command"];
	if(command.IsNull())
	{		
		Kernel_c::GetInstance().LogStream() << "[Phobos::Editor::EditorModule_c::OnFixedUpdate] JSON does not contains valid commands";
		return;
	}

	auto &requestFactory = RequestFactory_c::GetInstance();
	auto request = requestFactory.Create(command.GetString(), obj);
	request->Execute();
}

void Phobos::Editor::EditorModule_c::OnFixedUpdate()
{
	StringVector_t messages(clNetworkService.GetPendingMessages());

	Kernel_c &kernel = Kernel_c::GetInstance();	

	rapidjson::Document document;

	for(const std::string &msg : messages)
	{
		document.Parse<0>(msg.c_str());

		if(document.HasParseError())		
		{
			kernel.LogStream() << "[Phobos::Editor::EditorModule_c::OnFixedUpdate] Error parsing JSON: " << msg;
			continue;
		}

		const auto &commandList = document["commands"];
		if(!commandList.IsNull())
		{
			if(!commandList.IsArray())
			{
				kernel.LogStream() << "[Phobos::Editor::EditorModule_c::OnFixedUpdate] Command list is no an array: " << msg;
				continue;
			}

			for (rapidjson::Value::ConstValueIterator itr = commandList.Begin(); itr != commandList.End(); ++itr)
				this->ExecuteJsonCommand(*itr);
		}
		else
		{			
			this->ExecuteJsonCommand(document);
		}		
	}
}

