#include "PH_Client.h"

#include <PH_Console.h>
#include <PH_ContextUtils.h>
#include <PH_Error.h>
#include <PH_Exception.h>
#include <PH_Kernel.h>
#include <PH_WorldManager.h>

namespace Phobos
{
	PH_DEFINE_DEFAULT_SINGLETON(Client);

	Client_c::Client_c():
		CoreModule_c("Client"),
		cmdLoadMap("loadMap"),
		fMapLoaded(false)
	{		
		cmdLoadMap.SetProc(PH_CONTEXT_CMD_BIND(&Client_c::CmdLoadMap, this));
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
		}
		catch(Exception_c &ex)
		{
			Kernel_c::GetInstance().LogMessage(ex.what());
			fMapLoaded = false;
		}
	}
}

