#include "PH_BaseClient.h"

#include <PH_Console.h>
#include <PH_Render.h>

#include "PH_IPlayerCommandProducer.h"

namespace Phobos
{
	BaseClient_c::BaseClient_c(const char *name):
		CoreModule_c(name),
		varMouseClipped(PH_PLAYER_CMD_MOUSE_CLIPPED_VAR, "false")
	{
		//empty
	}

	void BaseClient_c::OnPrepareToBoot()
	{
		ConsolePtr_t console = Console_c::GetInstance();

		console->AddContextVar(varMouseClipped);
	}

	BaseClient_c::ConfigInfo_s BaseClient_c::GetConfig()
	{
		ConfigInfo_s info;

		info.pWindowHandler = Render_c::GetInstance()->GetWindowHandler();
		info.ipMouse = boost::static_pointer_cast<MouseInputDevice_c>(InputManager_c::GetInstance()->GetDevice(INPUT_DEVICE_MOUSE));

		return info;
	}

	void BaseClient_c::ClipMouseCursor()
	{
		ConfigInfo_s info = this->GetConfig();

		info.ipMouse->ClipToWindow(info.pWindowHandler);		
		
		varMouseClipped.SetValue("true");

		this->OnMouseClip();
	}

	void BaseClient_c::UnclipMouseCursor()
	{
		ConfigInfo_s info = this->GetConfig();

		info.ipMouse->Unclip();
		
		varMouseClipped.SetValue("false");

		this->OnMouseUnclip();
	}

	bool BaseClient_c::IsMouseClipped()
	{
		return varMouseClipped.GetBoolean();
	}
}
