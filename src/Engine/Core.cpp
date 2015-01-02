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
#include <Phobos/Memory.h>
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

namespace Phobos
{
	namespace Engine
	{		
		void ClocksRegisterCommands(Phobos::Shell::IContext &context);

		void ClocksTickFixedUpdate(Phobos::Chrono::Seconds seconds);
		void ClocksTickUpdate(Chrono::Seconds seconds, Float_t delta);
	}
}

Phobos::Engine::Core &Phobos::Engine::Core::CreateInstance(Shell::IContext &context, const char *cfgFileName, int argc, char * const argv[])
{
	PH_ASSERT_MSG(!g_pclInstance, "[Core::CreateInstance]: Instance already exists");

	g_pclInstance = PH_NEW Core(context, cfgFileName, argc, argv);

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

Phobos::Engine::Core::Core(Shell::IContext &context, const char *cfgFileName, int argc, char * const argv[]) :
	m_clModule("Core"),	
	m_cmdListModules("listModules"),
	m_cmdQuit("quit"),
	m_varFixedTime("dvFixedTime", "0"),
	m_varEngineFPS("dvEngineFPS", "60"),
	m_varMinFrameTime("dvMinFrameTime", "0.01"),
	m_fLaunchedBoot(false),
	m_fStopMainLoop(false)
{
	ObjectManager::AddObject(m_clModule, Path("/"));

	m_clModule.LaunchBootModule(cfgFileName, argc, argv);

	this->RegisterCommands(context);
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

Phobos::Chrono::Seconds Phobos::Engine::Core::GetUpdateTime()
{
	const Float_t updateTime = m_varEngineFPS.GetFloat();

	if(updateTime > 0)
		return(Chrono::Seconds(1.0f / updateTime));
	else
	{
		LogMakeStream() << "[Core::GetUpdateTime] Warning: Invalid update time: " << updateTime << ", must be > 0";
		m_varEngineFPS.SetValue("60");

		return Chrono::Seconds(1.0f / UPDATE_TIME);
	}
}

Phobos::Chrono::Seconds Phobos::Engine::Core::GetMinFrameTime()
{
	Float_t minFrameTime = m_varMinFrameTime.GetFloat();

	if(minFrameTime > 0.05)
	{
		LogMakeStream() << "[Core::GetMinFrameTime] Warning: Invalid minFrameTime: " << minFrameTime << ", must be < 0.05";
		m_varMinFrameTime.SetValue("0.01");

		minFrameTime = MIN_TIME;
	}

	return Chrono::Seconds(minFrameTime);
}

void Phobos::Engine::Core::StartMainLoop()
{
	m_clModule.Init();
	m_clModule.Start();	
	m_clModule.Started();

	Chrono::Seconds	executionTime;		
	auto			updateTime = GetUpdateTime();
	auto previousTime = Chrono::Clock::now();
			
	do
	{
		//this->SetFrameRate(updateTime.count());
						
		auto now = Chrono::Clock::now();
		auto lastFrameTime = now - previousTime;
		
		if(m_varFixedTime.GetBoolean())
			lastFrameTime = updateTime;

		if (lastFrameTime == Chrono::Seconds(0.0f))
		{				
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			continue;
		}					

		//how long the simulation can run?
		executionTime += lastFrameTime;
		auto totalFrameTime = lastFrameTime;

		#ifdef PH_DEBUG
			//this happens on debug mode while stopped on break points
		if (executionTime > Chrono::Seconds(20.0f))
			executionTime = updateTime;
		#endif				

		//update the game on fixed time steps
		while(executionTime >= updateTime)
		{
			//fixed Update
			this->FixedUpdate(updateTime);
				
			executionTime -= updateTime;
		}
		Float_t delta = executionTime / updateTime;			
			
		//Now update other modules as fast as we can
		this->Update(totalFrameTime, delta);

		//update it after frame
		updateTime = this->GetUpdateTime();
		//m_clTimer.SetMinInterval( GetMinFrameTime() );	

		previousTime = now;

	} while(!m_fStopMainLoop);

	m_clModule.Stop();
	m_clModule.Finalize();	
}

void Phobos::Engine::Core::Update(Chrono::Seconds seconds, Float_t delta)
{
	/*
	for(int i = 0;i < TimerTypes::MAX_TIMERS; ++i)
	{
		if(m_stSimInfo.m_stTimers[i].IsPaused())
			continue;

		m_stSimInfo.m_stTimers[i].m_fpRenderFrameTime = seconds;
		m_stSimInfo.m_stTimers[i].m_fpTotalRenderFrameTime += seconds;
		m_stSimInfo.m_stTimers[i].m_fpDelta = delta;
	}
	*/

	ClocksTickUpdate(seconds, delta);

	m_clModule.Update();	
}

void Phobos::Engine::Core::FixedUpdate(Chrono::Seconds seconds)
{
	ClocksTickFixedUpdate(seconds);

	m_clModule.FixedUpdate();
}

void Phobos::Engine::Core::RegisterCommands(Shell::IContext &context)
{
	m_cmdListModules.SetProc(PH_CONTEXT_CMD_BIND(&Core::CmdListModules, this));
	m_cmdQuit.SetProc(PH_CONTEXT_CMD_BIND(&Core::CmdQuit, this));
	
	context.AddContextCommand(m_cmdListModules);
	context.AddContextCommand(m_cmdQuit);

	context.AddContextVariable(m_varFixedTime);
	context.AddContextVariable(m_varEngineFPS);
	context.AddContextVariable(m_varMinFrameTime);

	ClocksRegisterCommands(context);
}

void Phobos::Engine::Core::CmdListModules(const Shell::StringVector_t &args, Shell::Context &)
{
	m_clModule.LogCoreModules();
}

void Phobos::Engine::Core::CmdQuit(const Shell::StringVector_t &, Shell::Context &)
{
	this->StopMainLoop();
}
