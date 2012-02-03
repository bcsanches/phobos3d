#include "PH_BaseClient.h"

#include <PH_ContextUtils.h>
#include <PH_Console.h>
#include <PH_Core.h>
#include <PH_Render.h>

#include "PH_IPlayerCommandProducer.h"
#include "PH_SpectatorCameraCommandProducer.h"

namespace Phobos
{
	BaseClient_c::BaseClient_c(const char *name):
		CoreModule_c(name),
		cmdToggleMouseCursorClip("toggleMouseCursorClip"),
		cmdNullMouseThumb(PH_MOUSE_THUMB_CMD_NAME),
		varMouseClipped(PH_PLAYER_CMD_MOUSE_CLIPPED_VAR, "false")
	{
		cmdNullMouseThumb.SetProc(PH_CONTEXT_CMD_BIND(&BaseClient_c::CmdNullMouseThumb, this));
		cmdToggleMouseCursorClip.SetProc(PH_CONTEXT_CMD_BIND(&BaseClient_c::CmdToggleMouseCursorClip, this));
	}

	void BaseClient_c::OnPrepareToBoot()
	{
		ConsolePtr_t console = Console_c::GetInstance();

		console->AddContextVar(varMouseClipped);		
	}

	void BaseClient_c::RegisterToggleMouseCursorClipCmd()
	{
		ConsolePtr_t console = Console_c::GetInstance();

		console->AddContextCmd(cmdToggleMouseCursorClip);
	}

	void BaseClient_c::RegisterNullMouseThumbCmd()
	{
		ConsolePtr_t console = Console_c::GetInstance();

		console->AddContextCmd(cmdNullMouseThumb);
	}

	void BaseClient_c::UnregisterNullMouseThumbCmd()
	{
		cmdNullMouseThumb.Unlink();
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

	void BaseClient_c::CmdToggleMouseCursorClip(const Phobos::StringVector_t &, Phobos::Context_c &  )
	{
		if(this->IsMouseClipped())
			this->UnclipMouseCursor();
		else
			this->ClipMouseCursor();
	}

	void BaseClient_c::CmdNullMouseThumb(const Phobos::StringVector_t &, Phobos::Context_c &  )
	{
		//empty
	}  
}
