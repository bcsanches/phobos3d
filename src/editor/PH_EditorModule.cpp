#include "Editor/PH_EditorModule.h"

#include <PH_Exception.h>
#include <PH_Kernel.h>
#include <PH_Session.h>

#include <json_spirit.h>

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

void Phobos::Editor::EditorModule_c::OnFixedUpdate()
{
	StringVector_t messages(clNetworkService.GetPendingMessages());

	Kernel_c &kernel = Kernel_c::GetInstance();
	auto &requestFactory = RequestFactory_c::GetInstance();

	for(const std::string &msg : messages)
	{
		json_spirit::mValue value;

		if(!json_spirit::read(msg, value))
		{
			kernel.LogStream() << "[Phobos::Editor::EditorModule_c::OnFixedUpdate] Error parsing JSON: " << msg;
			continue;
		}

		auto &obj = value.get_obj();

		auto it = obj.find("command");
		if(it == obj.end())
		{
			kernel.LogStream() << "[Phobos::Editor::EditorModule_c::OnFixedUpdate] Invalid JSON, no command: " << msg;
			continue;
		}

		auto request = requestFactory.Create(it->second.get_str(), obj);
		request->Execute();
	}
}

