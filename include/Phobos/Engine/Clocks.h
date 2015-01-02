#ifndef PH_CLOCKS_H
#define PH_CLOCKS_H

#include <Phobos/System/Chrono.h>

#include "Phobos/Engine/EngineAPI.h"

namespace Phobos
{
	namespace Engine
	{
		class SystemClock
		{
			public:
				typedef Float_t rep;
				typedef Chrono::Clock::period period;
				typedef Chrono::Clock::duration duration;
				typedef Chrono::chrono::time_point<SystemClock> time_point;
				static const bool is_monotonic = false;
				static const bool is_steady = false;

				PH_ENGINE_API static time_point Now();

				PH_ENGINE_API static duration GetFrameDuration();
		};

		class GameClock
		{
			public:
				typedef Float_t rep;
				typedef Chrono::Clock::period period;
				typedef Chrono::Clock::duration duration;
				typedef Chrono::chrono::time_point<SystemClock> time_point;
				static const bool is_monotonic = false;
				static const bool is_steady = false;

				PH_ENGINE_API static time_point Now();

				PH_ENGINE_API static duration GetFrameDuration();

				PH_ENGINE_API static bool IsPaused();
		};

		class GuiClock
		{
			public:
				typedef Float_t rep;
				typedef Chrono::Clock::period period;
				typedef Chrono::Clock::duration duration;
				typedef Chrono::chrono::time_point<SystemClock> time_point;
				static const bool is_monotonic = false;
				static const bool is_steady = false;

				PH_ENGINE_API static time_point Now();

				PH_ENGINE_API static duration GetFrameDuration();
		};

		class RenderClock
		{
			public:
			typedef Float_t rep;
			typedef Chrono::Clock::period period;
			typedef Chrono::Clock::duration duration;
			typedef Chrono::chrono::time_point<SystemClock> time_point;
			static const bool is_monotonic = false;
			static const bool is_steady = false;

			PH_ENGINE_API static time_point Now();

			PH_ENGINE_API static duration GetFrameDuration();

			PH_ENGINE_API static Float_t GetDelta();
		};
	}
}

#endif