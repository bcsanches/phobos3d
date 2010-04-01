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

namespace Phobos
{
	const String_c Core_c::DEFAULT_NAME = "core";
	CorePtr_t Core_c::ipInstance_gl;

	CorePtr_t Core_c::CreateInstance()
	{
		PH_ASSERT_MSG(!ipInstance_gl, "[Core_c::CreateInstance]: Instance already exists");

		ipInstance_gl = CorePtr_t(new Core_c(DEFAULT_NAME));

		Kernel_c::GetInstance().AddObject(ipInstance_gl, Path_c(PH_SYSTEM_FOLDER));

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
		Node_c(name),
		cmdTime("time"),
		cmdListModules("listModules"),
		cmdToggleTimerPause("toggleTimerPause")
	{
		MemoryZero(&stSimInfo, sizeof(stSimInfo));
	}

	void Core_c::Update(Float_t seconds, Float_t delta)
	{
	}

	void Core_c::FixedUpdate(Float_t seconds)
	{
	}


	void Core_c::AddModule(CoreModulePtr_t module, UInt32_t priority)
	{
		this->AddChild(module);		

		vecModules.push_back(ModuleInfo_s(module, priority));	
		std::sort(vecModules.begin(), vecModules.end());
	}

	void Core_c::AddModuleToDestroyList(CoreModulePtr_t module)
	{
		bool found = false;
		for(ModulesVector_t::iterator it = vecModules.begin(), end = vecModules.end(); it != end; ++it)
		{		
			if(it->ipModule == module)
			{
				vecModules.erase(it);
				found = true;
				break;
			}
		}

		if(!found)
		{
			std::stringstream stream;
			stream << "Module " << module->GetName() << " not found on core list.";

			PH_RAISE(OBJECT_NOT_FOUND_EXCEPTION, "Core_c::AddModuleToDestroyList", stream.str());
		}

		setModulesToDestroy.insert(module);
	}

	void Core_c::CreateDefaultCmds(Context_c &context)
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
			Kernel_c::GetInstance().LogMessage("IM_Core_c::PauseTimer: Cant pause sys timer");
			return;
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
				<<"\tFrameCount: "<<stSimInfo.stTimers[CORE_SYS_TIMER].uFrameCount<<endl;
		
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
			stream << endl;

			Kernel_c::GetInstance().LogMessage(stream.str());
			return;
		}

		const String_c &timerName(args[1]);		

		for(int i = 0;pauseInfo[i].pstrzName; ++i)
		{
			if(timerName.compare(pauseInfo[i].pstrzName) == 0)
			{
				this->ToggleTimerPause(pauseInfo[i].eType);

				break;
			}
		}
	}

	void Core_c::CmdListModules(const StringVector_t &args, Context_c &)
	{
		using namespace std;
		stringstream stream;

		stream << "Core modules: " << endl;		
		BOOST_FOREACH(ModuleInfo_s &m, vecModules)
		{			
			stream << '\t' << m.ipModule->GetName() << ' ' << m.u32Priority << endl;			
		}

		Kernel_c::GetInstance().LogMessage(stream.str());	
	}
}
