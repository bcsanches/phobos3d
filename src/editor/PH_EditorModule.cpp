#include "PH_EditorModule.h"

#include <PH_Exception.h>
#include <PH_Session.h>

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
	session.SetForm(NULL);
}
