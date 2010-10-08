#include "PH_Client.h"

#include <PH_Console.h>
#include <PH_ContextUtils.h>
#include <PH_Error.h>
#include <PH_Exception.h>
#include <PH_Kernel.h>
#include <PH_MouseInputDevice.h>
#include <PH_Render.h>
#include <PH_Window.h>
#include <PH_WorldManager.h>

namespace Phobos
{
	PH_DEFINE_DEFAULT_SINGLETON(Client);

	Client_c::Client_c():
		CoreModule_c("Client"),
		cmdLoadMap("loadMap"),
		cmdNullMouseThumb(PH_MOUSE_THUMB_CMD_NAME),
		cmdToggleMouseCursorClip("toggleMouseCursorClip"),
		varMouseSensitivity("dvMouseSensitivity", "0.1"),
		varSpectatorMoveSpeed("dvSpectatorMoveSpeed", "2.0"),
		varSpectatorTurnSpeed("dvSpectatorTurnSpeed", "45.0"),
		fMapLoaded(false),
		fMouseClipped(false)
	{		
		cmdLoadMap.SetProc(PH_CONTEXT_CMD_BIND(&Client_c::CmdLoadMap, this));

		cmdToggleMouseCursorClip.SetProc(PH_CONTEXT_CMD_BIND(&Client_c::CmdToggleMouseCursorClip, this));
		cmdNullMouseThumb.SetProc(PH_CONTEXT_CMD_BIND(&Client_c::CmdNullMouseThumb, this));
	}

	void Client_c::OnRenderReady()
	{		
	}

	void Client_c::OnFixedUpdate()
	{
		if(!fMapLoaded)
			return;

		clSpectatorCamera.FixedUpdate();
	}

	void Client_c::OnUpdate()
	{
		if(!fMapLoaded)
			return;

		clSpectatorCamera.Update();
	}

	void Client_c::OnPrepareToBoot()
	{
		ConsolePtr_t console = Console_c::GetInstance();

		console->AddContextCmd(cmdLoadMap);
		console->AddContextCmd(cmdToggleMouseCursorClip);
		console->AddContextCmd(cmdNullMouseThumb);
	}

	struct ConfigInfo_s
	{
		MouseInputDevicePtr_t ipMouse;
		void *pWindowHandler;
	};

	ConfigInfo_s Client_c::GetConfig()
	{
		ConfigInfo_s info;

		info.pWindowHandler = Render_c::GetInstance()->GetWindowHandler();
		info.ipMouse = boost::static_pointer_cast<MouseInputDevice_c>(InputManager_c::GetInstance()->GetDevice(INPUT_DEVICE_MOUSE));

		return info;
	}
	
	void Client_c::ClipMouseCursor()
	{		
		ConfigInfo_s info = this->GetConfig();

		info.ipMouse->ClipToWindow(info.pWindowHandler);

		cmdNullMouseThumb.Unlink();
		clSpectatorCamera.EnableMouse();
		fMouseClipped = true;
	}

	void Client_c::UnclipMouseCursor()
	{
		ConfigInfo_s info = this->GetConfig();

		info.ipMouse->Unclip();

		clSpectatorCamera.DisableMouse();
		Console_c::GetInstance()->AddContextCmd(cmdNullMouseThumb);
		fMouseClipped = false;
	}

	void Client_c::CmdNullMouseThumb(const StringVector_t &, Context_c &  )
	{
		//empty
	}

	void Client_c::CmdToggleMouseCursorClip(const StringVector_t &, Context_c &  )
	{
		if(fMouseClipped)
			this->UnclipMouseCursor();
		else
			this->ClipMouseCursor();		
	}

	void Client_c::CmdLoadMap(const StringVector_t &args, Context_c &)
	{
		if(args.size() < 2)
		{
			Kernel_c::GetInstance().LogMessage("[CmdLoadMap] Insuficient parameters, usage: loadMap <mapName>");

			return;
		}

		try
		{
			WorldManager_c::GetInstance()->LoadMap(args[1]);

			fMapLoaded = true;
			clSpectatorCamera.EnableController();

			if(!fMouseClipped)
				this->ClipMouseCursor();
		}
		catch(Exception_c &ex)
		{
			Kernel_c::GetInstance().LogMessage(ex.what());
			fMapLoaded = false;
		}
	}
}

