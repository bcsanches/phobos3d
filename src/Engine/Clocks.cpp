#include "Phobos/Engine/Clocks.h"

#include <Phobos/Error.h>
#include <Phobos/Exception.h>
#include <Phobos/Log.h>

#include <Phobos/Shell/Context.h>

namespace
{
	namespace ClockTypes
	{
		enum Enum
		{
			SYSTEM,
			RENDER,
			GAME,
			GUI,

			NUM_TYPES
		};
	}

	struct ClockImpl
	{
		Phobos::System::Seconds m_durTotal;
		Phobos::System::Seconds m_durFrame;

		Phobos::Float_t m_fpDelta = 0.0f;

		bool m_fPaused = false;
	};

	static void CmdTime(const Phobos::Shell::StringVector_t &args, Phobos::Shell::Context &);
	static void CmdToggleClockPause(const Phobos::Shell::StringVector_t &args, Phobos::Shell::Context &);

	static ClockImpl g_stClocks[ClockTypes::NUM_TYPES];
	static Phobos::Shell::Command	g_cmdTime("time", CmdTime);
	static Phobos::Shell::Command	g_cmdToggleClockPause("toggleClockPause", CmdToggleClockPause);

	static void PauseClock(ClockTypes::Enum clock)
	{
		PH_ASSERT(clock < ClockTypes::NUM_TYPES);

		if ((clock == ClockTypes::SYSTEM) || (clock == ClockTypes::RENDER))
		{
			PH_RAISE(Phobos::INVALID_PARAMETER_EXCEPTION, "Engine::PauseClock", "Cant pause this timer");
		}

		g_stClocks[clock].m_fPaused = true;
	}

	static void UnpauseTimer(ClockTypes::Enum clock)
	{
		PH_ASSERT(clock < ClockTypes::NUM_TYPES);

		g_stClocks[clock].m_fPaused = false;
	}

	static void ToggleClockPause(ClockTypes::Enum clock)
	{
		PH_ASSERT(clock < ClockTypes::NUM_TYPES);

		if ((clock == ClockTypes::SYSTEM) || (clock == ClockTypes::RENDER))
		{
			Phobos::LogMessage("Engine::ToggleClockPause: Cant pause this timer");
			return;
		}

		g_stClocks[clock].m_fPaused = !g_stClocks[clock].m_fPaused;
	}	

	static void CmdTime(const Phobos::Shell::StringVector_t &args, Phobos::Shell::Context &)
	{
		using namespace std;
		stringstream stream;

		stream << "[Engine::Clocks] Current time:" << endl << "\tfpTotalTics: " << g_stClocks[ClockTypes::SYSTEM].m_durTotal << endl
			<< "\tfpFrameTime: " << g_stClocks[ClockTypes::SYSTEM].m_durFrame << endl;
			//<< "\tFrameCount: " << m_stSimInfo.m_stTimers[TimerTypes::SYSTEM].m_uFrameCount;

		Phobos::LogMessage(stream.str());
	}

	static void CmdToggleClockPause(const Phobos::Shell::StringVector_t &args, Phobos::Shell::Context &)
	{
		struct PauseInfo_s
		{
			const char			*pstrzName;
			ClockTypes::Enum	eType;
		};

		PauseInfo_s pauseInfo[] =
		{
			{ "SYSTEM", ClockTypes::SYSTEM },
			{ "RENDER", ClockTypes::RENDER },
			{ "GAME", ClockTypes::GAME },
			{ "UI", ClockTypes::GUI },
			{ NULL, ClockTypes::NUM_TYPES }
		};

		if (args.size() < 2)
		{
			using namespace std;
			stringstream stream;

			stream << "[Engine::CmdToggleClockPause] togglePause usage error, usage: togglePause ";
			for (int i = 0; pauseInfo[i].pstrzName; ++i)
			{
				stream << pauseInfo[i].pstrzName << '|';
			}

			Phobos::LogMessage(stream.str());			
		}
		else
		{
			const Phobos::String_t &timerName(args[1]);

			for (int i = 0; pauseInfo[i].pstrzName; ++i)
			{
				if (timerName.compare(pauseInfo[i].pstrzName) == 0)
				{
					ToggleClockPause(pauseInfo[i].eType);

					std::stringstream stream;
					stream << "[Engine::CmdToggleClockPause] " << (g_stClocks[pauseInfo[i].eType].m_fPaused ? "Paused" : "Unpaused") << " " << pauseInfo[i].pstrzName;
					Phobos::LogMessage(stream.str());

					break;
				}
			}
		}				
	}		
}

namespace Phobos
{
	namespace Engine
	{
		extern void ClocksRegisterCommands(Phobos::Shell::IContext &context)
		{
			context.AddContextCommand(g_cmdTime);
			context.AddContextCommand(g_cmdToggleClockPause);
		}			

		extern void ClocksTickFixedUpdate(System::Seconds seconds)
		{
			for (auto &clock : g_stClocks)
			{
				if (&clock == &g_stClocks[ClockTypes::RENDER])
					continue;

				clock.m_durTotal += seconds;
				clock.m_durFrame = seconds;
			}
		}

		extern void ClocksTickUpdate(System::Seconds seconds, Float_t delta)
		{
			g_stClocks[ClockTypes::RENDER].m_durTotal += seconds;
			g_stClocks[ClockTypes::RENDER].m_durFrame = seconds;

			g_stClocks[ClockTypes::RENDER].m_fpDelta = delta;

		}

		SystemClock::time_point SystemClock::Now()
		{
			return time_point(g_stClocks[ClockTypes::SYSTEM].m_durTotal);
		}

		SystemClock::duration SystemClock::GetFrameDuration()
		{
			return g_stClocks[ClockTypes::SYSTEM].m_durFrame;
		}

		RenderClock::time_point RenderClock::Now()
		{
			return time_point(g_stClocks[ClockTypes::RENDER].m_durTotal);
		}

		RenderClock::duration RenderClock::GetFrameDuration()
		{
			return g_stClocks[ClockTypes::RENDER].m_durFrame;
		}

		Float_t RenderClock::GetDelta()
		{
			return g_stClocks[ClockTypes::RENDER].m_fpDelta;
		}

		GameClock::time_point GameClock::Now()
		{
			return time_point(g_stClocks[ClockTypes::GAME].m_durTotal);
		}

		GameClock::duration GameClock::GetFrameDuration()
		{
			return g_stClocks[ClockTypes::GAME].m_durFrame;
		}

		bool GameClock::IsPaused()
		{
			return g_stClocks[ClockTypes::GAME].m_fPaused;
		}

		GuiClock::time_point GuiClock::Now()
		{
			return time_point(g_stClocks[ClockTypes::GUI].m_durTotal);
		}

		GuiClock::duration GuiClock::GetFrameDuration()
		{
			return g_stClocks[ClockTypes::GUI].m_durFrame;
		}
	}
}

#if 0

void Phobos::Engine::Core::ResetTimer(TimerTypes_t timer)
{
	PH_ASSERT(timer < TimerTypes::MAX_TIMERS);

	m_stSimInfo.m_stTimers[timer].Reset();
}

#endif
