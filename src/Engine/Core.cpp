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

#include "Phobos/Engine/Core.h"

#include <sstream>

#include <chrono>
#include <thread>

#include <Phobos/Error.h>
#include <Phobos/Exception.h>
#include <Phobos/Folders.h>
#include <Phobos/Log.h>
#include <Phobos/MemoryFunctions.h>
#include <Phobos/ObjectManager.h>
#include <Phobos/Path.h>
#include <Phobos/Types.h>

#include <Phobos/Shell/Context.h>
#include <Phobos/Shell/Utils.h>

#include "Phobos/Engine/Module.h"
#include "Phobos/Engine/BootModule.h"

#define UPDATE_TIME (1.0f / 60.0f)
#define MIN_TIME (10.0f / 1000.0f)

static Phobos::Engine::Core *g_pclInstance = nullptr;

Phobos::Engine::Core &Phobos::Engine::Core::CreateInstance(const char *cfgFileName, int argc, char * const argv[])
{
	PH_ASSERT_MSG(!g_pclInstance, "[Core::CreateInstance]: Instance already exists");

	g_pclInstance = new Core(cfgFileName, argc, argv);

	return *g_pclInstance;
}

void Phobos::Engine::Core::ReleaseInstance()
{
	PH_ASSERT_MSG(g_pclInstance, "[Core::ReleaseInstance]: Instance does not exists, use CreateInstance");

	delete g_pclInstance;

	g_pclInstance = nullptr;
}

Phobos::Engine::Core &Phobos::Engine::Core::GetInstance()
{
	PH_ASSERT_MSG(g_pclInstance, "[Core::GetInstance]: Instance does not exists, use CreateInstance");

	return *g_pclInstance;
}

Phobos::Engine::Core::Core(const char *cfgFileName, int argc, char * const argv[]) :
	m_clModule("Core"),
	m_cmdTime("time"),
	m_cmdToggleTimerPause("toggleTimerPause"),
	m_cmdListModules("listModules"),
	m_cmdQuit("quit"),
	m_varFixedTime("dvFixedTime", "0"),
	m_varEngineFPS("dvEngineFPS", "60"),
	m_varMinFrameTime("dvMinFrameTime", "0.01"),
	m_fLaunchedBoot(false),
	m_fStopMainLoop(false)
{
	MemoryZero(&m_stSimInfo, sizeof(m_stSimInfo));

	ObjectManager::AddObject(m_clModule, Path("/"));

	m_clModule.LaunchBootModule(cfgFileName, argc, argv);
}

Phobos::Engine::Core::~Core()
{
	//empty
}

void Phobos::Engine::Core::StopMainLoop()
{
	m_fStopMainLoop = true;
}

void Phobos::Engine::Core::AddModule(Module &module, UInt32_t priority)
{
	m_clModule.AddModule(module, priority);
}

void Phobos::Engine::Core::RemoveModule(Module &module)
{
	m_clModule.RemoveModule(module);
}

Phobos::Float_t Phobos::Engine::Core::GetUpdateTime(void)
{
	const Float_t updateTime = m_varEngineFPS.GetFloat();

	if(updateTime > 0)
		return(1.0f / updateTime);
	else
	{
		LogMakeStream() << "[Core::GetUpdateTime] Warning: Invalid update time: " << updateTime << ", must be > 0";
		m_varEngineFPS.SetValue("60");

		return(1.0f / UPDATE_TIME);
	}
}

Phobos::Float_t Phobos::Engine::Core::GetMinFrameTime(void)
{
	Float_t minFrameTime = m_varMinFrameTime.GetFloat();

	if(minFrameTime > 0.05)
	{
		LogMakeStream() << "[Core::GetMinFrameTime] Warning: Invalid minFrameTime: " << minFrameTime << ", must be < 0.05";
		m_varMinFrameTime.SetValue("0.01");

		minFrameTime = MIN_TIME;
	}

	return minFrameTime;
}

void Phobos::Engine::Core::StartMainLoop()
{
	m_clModule.PreInit();
	m_clModule.Init();
	m_clModule.Start();	
	m_clModule.Started();

	Float_t			executionTime = 0;		
	Float_t			updateTime = GetUpdateTime();
		
	m_clTimer.Reset();
	do
	{
		this->SetFrameRate(updateTime);
						
		auto lastFrameTime = m_clTimer.Elapsed();		

		//FIXME HACK, fIX THIS
		if(lastFrameTime > 1)
		{
			m_clTimer.Reset();
			continue;
		}

		if(m_varFixedTime.GetBoolean())
			lastFrameTime = updateTime;

		if(lastFrameTime == 0.0f)
		{				
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			continue;
		}					

		//how long the simulation can run?
		executionTime += lastFrameTime;
		auto totalFrameTime = lastFrameTime;

		#ifdef PH_DEBUG
			//this happens on debug mode while stopped on break points
			if(executionTime > 20)
				executionTime = updateTime;
		#endif				

		//update the game on fixed time steps
		while(executionTime >= updateTime)
		{
			//fixed Update
			this->FixedUpdate(updateTime);
				
			executionTime -= updateTime;
		}
		Float_t delta = (Float_t) executionTime / (Float_t) updateTime;			
			
		//Now update other modules as fast as we can
		this->Update(totalFrameTime, delta);

		//update it after frame
		updateTime = this->GetUpdateTime();
		m_clTimer.SetMinInterval( GetMinFrameTime() );				

	} while(!m_fStopMainLoop);

	m_clModule.Stop();
	m_clModule.Finalize();	
}

void Phobos::Engine::Core::Update(Float_t seconds, Float_t delta)
{
	for(int i = 0;i < TimerTypes::MAX_TIMERS; ++i)
	{
		if(m_stSimInfo.m_stTimers[i].IsPaused())
			continue;

		m_stSimInfo.m_stTimers[i].m_fpRenderFrameTime = seconds;
		m_stSimInfo.m_stTimers[i].m_fpTotalRenderFrameTime += seconds;
		m_stSimInfo.m_stTimers[i].m_fpDelta = delta;
	}

	m_clModule.Update();	
}

void Phobos::Engine::Core::FixedUpdate(Float_t seconds)
{
	for(int i = 0;i < TimerTypes::MAX_TIMERS; ++i)
	{
		if(m_stSimInfo.m_stTimers[i].IsPaused())
			continue;

		m_stSimInfo.m_stTimers[i].m_fpFrameTime = seconds;
		m_stSimInfo.m_stTimers[i].m_fpTotalTicks += seconds;
		++m_stSimInfo.m_stTimers[i].m_uFrameCount;
	}

	m_clModule.FixedUpdate();
}

void Phobos::Engine::Core::RegisterCommands(Shell::IContext &context)
{
	m_cmdTime.SetProc(PH_CONTEXT_CMD_BIND(&Core::CmdTime, this));
	m_cmdToggleTimerPause.SetProc(PH_CONTEXT_CMD_BIND(&Core::CmdToggleTimerPause, this));
	m_cmdListModules.SetProc(PH_CONTEXT_CMD_BIND(&Core::CmdListModules, this));
	m_cmdQuit.SetProc(PH_CONTEXT_CMD_BIND(&Core::CmdQuit, this));

	context.AddContextCommand(m_cmdTime);
	context.AddContextCommand(m_cmdToggleTimerPause);
	context.AddContextCommand(m_cmdListModules);
	context.AddContextCommand(m_cmdQuit);

	context.AddContextVariable(m_varFixedTime);
	context.AddContextVariable(m_varEngineFPS);
	context.AddContextVariable(m_varMinFrameTime);
}

void Phobos::Engine::Core::PauseTimer(TimerTypes_t timer)
{
	PH_ASSERT(timer < TimerTypes::MAX_TIMERS);

	if(timer == TimerTypes::SYSTEM)
	{
		PH_RAISE(INVALID_PARAMETER_EXCEPTION, "Core::PauseTimer", "Cant pause sys timer");
	}

	m_stSimInfo.m_stTimers[timer].Pause();
}

void Phobos::Engine::Core::UnpauseTimer(TimerTypes_t timer)
{
	PH_ASSERT(timer < TimerTypes::MAX_TIMERS);

	m_stSimInfo.m_stTimers[timer].Unpause();
}

void Phobos::Engine::Core::ToggleTimerPause(TimerTypes_t timer)
{
	PH_ASSERT(timer < TimerTypes::MAX_TIMERS);

	if(timer == TimerTypes::SYSTEM)
	{
		LogMessage("IM_Core::PauseTimer: Cant pause sys timer");
		return;
	}

	m_stSimInfo.m_stTimers[timer].TogglePause();
}

void Phobos::Engine::Core::ResetTimer(TimerTypes_t timer)
{
	PH_ASSERT(timer < TimerTypes::MAX_TIMERS);

	m_stSimInfo.m_stTimers[timer].Reset();
}

void Phobos::Engine::Core::CmdTime(const Shell::StringVector_t &args, Shell::Context &)
{
	using namespace std;
	stringstream stream;

	stream	<< "[IM_Core] Current time:"<<endl<<"\tfpTotalTics: "<<m_stSimInfo.m_stTimers[TimerTypes::SYSTEM].m_fpTotalTicks<<endl
			<<"\tfpFrameTime: "<<m_stSimInfo.m_stTimers[TimerTypes::SYSTEM].m_fpFrameTime<<endl
			<<"\tFrameCount: "<<m_stSimInfo.m_stTimers[TimerTypes::SYSTEM].m_uFrameCount;

	LogMessage(stream.str());
}

void Phobos::Engine::Core::CmdToggleTimerPause(const Shell::StringVector_t &args, Shell::Context &)
{
	struct PauseInfo_s
	{
		const char	*pstrzName;
		TimerTypes_t eType;
	};

	PauseInfo_s pauseInfo[] =
	{
		{"SYSTEM",	TimerTypes::SYSTEM},
		{"GAME",	TimerTypes::GAME},
		{"UI",		TimerTypes::UI},
		{NULL,		TimerTypes::MAX_TIMERS}
	};

	if(args.size() < 2)
	{
		using namespace std;
		stringstream stream;

		stream << "[Core::CmdToggleTimerPause] togglePause usage error, usage: togglePause ";
		for(int i = 0;pauseInfo[i].pstrzName; ++i)
		{
			stream << pauseInfo[i].pstrzName << '|';
		}

		LogMessage(stream.str());
		return;
	}

	const String_t &timerName(args[1]);

	for(int i = 0;pauseInfo[i].pstrzName; ++i)
	{
		if(timerName.compare(pauseInfo[i].pstrzName) == 0)
		{				
			this->ToggleTimerPause(pauseInfo[i].eType);

			std::stringstream stream;
			stream << "[Core::CmdToggleTimerPause] " << (m_stSimInfo.m_stTimers[pauseInfo[i].eType].IsPaused() ? "Paused" : "Unpaused") << " " << pauseInfo[i].pstrzName;				
			LogMessage(stream.str());				
			break;
		}
	}
}

void Phobos::Engine::Core::CmdListModules(const Shell::StringVector_t &args, Shell::Context &)
{
	m_clModule.LogCoreModules();
}

void Phobos::Engine::Core::CmdQuit(const Shell::StringVector_t &, Shell::Context &)
{
	this->StopMainLoop();
}
