/*
Phobos 3d
April 2010
Copyright (c) 2005-2011 Bruno Sanches  http://code.google.com/p/phobos3d

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#include "PH_Core.h"

#include <boost/foreach.hpp>
#include <sstream>

#include <PH_Context.h>
#include <PH_ContextUtils.h>
#include <PH_Error.h>
#include <PH_Exception.h>
#include <PH_Folders.h>
#include <PH_Kernel.h>
#include <PH_MemoryFunctions.h>
#include <PH_Path.h>

#include "PH_CoreModule.h"
#include "PH_BootModule.h"

namespace Phobos
{
	const String_c Core_c::DEFAULT_NAME = "Core";
	CorePtr_t Core_c::ipInstance_gl;

	CorePtr_t Core_c::CreateInstance()
	{
		PH_ASSERT_MSG(!ipInstance_gl, "[Core_c::CreateInstance]: Instance already exists");

		ipInstance_gl = CorePtr_t(new Core_c(DEFAULT_NAME));

		Kernel_c::GetInstance().AddObject(ipInstance_gl, Path_c("/"));

		return ipInstance_gl;
	}

	void Core_c::ReleaseInstance()
	{
		PH_ASSERT_MSG(ipInstance_gl, "[Core_c::ReleaseInstance]: Instance does not exists, use CreateInstance");

		ipInstance_gl->RemoveSelf();
		ipInstance_gl.reset();
	}

	CorePtr_t Core_c::GetInstance()
	{
		PH_ASSERT_MSG(ipInstance_gl, "[Core_c::GetInstance]: Instance does not exists, use CreateInstance");

		return ipInstance_gl;
	}

	Core_c::Core_c(const Phobos::String_c &name):
		CoreModuleManager_c(name, PRIVATE_CHILDREN),
		cmdTime("time"),
		cmdToggleTimerPause("toggleTimerPause"),
		cmdListModules("listModules"),
		fLaunchedBoot(false)
	{
		MemoryZero(&stSimInfo, sizeof(stSimInfo));
	}

	Core_c::~Core_c()
	{
		//empty
	}

	void Core_c::Shutdown()
	{
		this->OnFinalize();
	}

	void Core_c::Update(Float_t seconds, Float_t delta)
	{
		for(int i = 0;i < CORE_MAX_TIMERS; ++i)
		{
			if(stSimInfo.stTimers[i].IsPaused())
				continue;

			stSimInfo.stTimers[i].fpRenderFrameTime = seconds;
			stSimInfo.stTimers[i].fpTotalRenderFrameTime += seconds;
			stSimInfo.stTimers[i].fpDelta = delta;
		}

		CoreModuleManager_c::OnUpdate();
	}

	void Core_c::FixedUpdate(Float_t seconds)
	{
		for(int i = 0;i < CORE_MAX_TIMERS; ++i)
		{
			if(stSimInfo.stTimers[i].IsPaused())
				continue;

			stSimInfo.stTimers[i].fpFrameTime = seconds;
			stSimInfo.stTimers[i].fpTotalTicks += seconds;
			++stSimInfo.stTimers[i].uFrameCount;
		}

		CoreModuleManager_c::OnFixedUpdate();
	}

	void Core_c::RegisterCommands(IContext_c &context)
	{
		cmdTime.SetProc(PH_CONTEXT_CMD_BIND(&Core_c::CmdTime, this));
		cmdToggleTimerPause.SetProc(PH_CONTEXT_CMD_BIND(&Core_c::CmdToggleTimerPause, this));
		cmdListModules.SetProc(PH_CONTEXT_CMD_BIND(&Core_c::CmdListModules, this));

		context.AddContextCmd(cmdTime);
		context.AddContextCmd(cmdToggleTimerPause);
		context.AddContextCmd(cmdListModules);
	}

	void Core_c::PauseTimer(CoreTimerTypes_e timer)
	{
		PH_ASSERT(timer < CORE_MAX_TIMERS);

		if(timer == CORE_SYS_TIMER)
		{
			PH_RAISE(INVALID_PARAMETER_EXCEPTION, "Core_c::PauseTimer", "Cant pause sys timer");
		}

		stSimInfo.stTimers[timer].Pause();
	}

	void Core_c::UnpauseTimer(CoreTimerTypes_e timer)
	{
		PH_ASSERT(timer < CORE_MAX_TIMERS);

		stSimInfo.stTimers[timer].Unpause();
	}

	void Core_c::ToggleTimerPause(CoreTimerTypes_e timer)
	{
		PH_ASSERT(timer < CORE_MAX_TIMERS);

		if(timer == CORE_SYS_TIMER)
		{
			Kernel_c::GetInstance().LogMessage("IM_Core_c::PauseTimer: Cant pause sys timer");
			return;
		}

		stSimInfo.stTimers[timer].TogglePause();
	}

	void Core_c::ResetTimer(CoreTimerTypes_e timer)
	{
		PH_ASSERT(timer < CORE_MAX_TIMERS);

		stSimInfo.stTimers[CORE_GAME_TIMER].Reset();
	}

	void Core_c::CmdTime(const StringVector_t &args, Context_c &)
	{
		using namespace std;
		stringstream stream;

		stream	<< "[IM_Core] Current time:"<<endl<<"\tfpTotalTics: "<<stSimInfo.stTimers[CORE_SYS_TIMER].fpTotalTicks<<endl
				<<"\tfpFrameTime: "<<stSimInfo.stTimers[CORE_SYS_TIMER].fpFrameTime<<endl
				<<"\tFrameCount: "<<stSimInfo.stTimers[CORE_SYS_TIMER].uFrameCount;

		Kernel_c::GetInstance().LogMessage(stream.str());
	}

	void Core_c::CmdToggleTimerPause(const StringVector_t &args, Context_c &)
	{
		struct PauseInfo_s
		{
			const char *pstrzName;
			CoreTimerTypes_e eType;
		};

		PauseInfo_s pauseInfo[] =
		{
			{"SYSTEM",	CORE_SYS_TIMER},
			{"GAME",	CORE_GAME_TIMER},
			{"UI",		CORE_UI_TIMER},
			{NULL,		CORE_MAX_TIMERS}
		};

		if(args.size() < 2)
		{
			using namespace std;
			stringstream stream;

			stream << "[Core_c::CmdToggleTimerPause] togglePause usage error, usage: togglePause ";
			for(int i = 0;pauseInfo[i].pstrzName; ++i)
			{
				stream << pauseInfo[i].pstrzName << '|';
			}

			Kernel_c::GetInstance().LogMessage(stream.str());
			return;
		}

		const String_c &timerName(args[1]);

		for(int i = 0;pauseInfo[i].pstrzName; ++i)
		{
			if(timerName.compare(pauseInfo[i].pstrzName) == 0)
			{				
				this->ToggleTimerPause(pauseInfo[i].eType);

				std::stringstream stream;
				stream << "[Core_c::CmdToggleTimerPause] " << (stSimInfo.stTimers[pauseInfo[i].eType].IsPaused() ? "Paused" : "Unpaused") << " " << pauseInfo[i].pstrzName;				
				Kernel_c::GetInstance().LogMessage(stream.str());				
				break;
			}
		}
	}

	void Core_c::CmdListModules(const StringVector_t &args, Context_c &)
	{
		this->LogCoreModules();
	}
}
