/*
Phobos 3d
April 2010
Copyright (c) 2005-2013 Bruno Sanches  http://code.google.com/p/phobos3d

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef PH_CORE_H
#define PH_CORE_H

#include <set>
#include <vector>

#include <Phobos/Shell/IContext.h>
#include <Phobos/Shell/Command.h>
#include <Phobos/Shell/Variable.h>

#include <Phobos/System/Timer.h>

#include "Phobos/Engine/ModuleManager.h"
#include "Phobos/Engine/EngineAPI.h"

namespace Phobos
{	
	namespace Engine
	{		
		class PH_ENGINE_API Core
		{
			public:
				struct TimerTypes
				{
					enum Enum
					{
						SYSTEM,
						GAME,
						UI,
		
						MAX_TIMERS
					};
				};

				typedef TimerTypes::Enum TimerTypes_t;

				enum TimerFlags
				{					
					PAUSED = 0x01					
				};

				struct Timer_s
				{
					//* Number of ticks occurred since started.
					Float_t	m_fpTotalTicks;

					//* Number of ticks for simulation frame
					Float_t	m_fpFrameTime;

					//* number of ticks for the rendering frame
					Float_t	m_fpRenderFrameTime;


					Float_t	m_fpTotalRenderFrameTime;

					//*number of frames occurred
					unsigned int m_uFrameCount;

					//*delta used for rendering interpolation
					Float_t	m_fpDelta;

					UInt32_t m_u32Flags;

					inline bool IsPaused(void) const
					{
						return(m_u32Flags & TimerFlags::PAUSED);
					}

					inline void Pause(void)
					{
						m_u32Flags |= TimerFlags::PAUSED;
					}

					inline void Unpause(void)
					{
						m_u32Flags &= ~TimerFlags::PAUSED;
					}

					inline void TogglePause(void)
					{
						if(this->IsPaused())
							this->Unpause();
						else
							this->Pause();
					}

					inline void Reset(void)
					{
						m_fpTotalTicks = 0;
						m_fpFrameTime = 0;
						m_fpRenderFrameTime = 0;
						m_fpTotalRenderFrameTime = 0;
						m_uFrameCount = 0;
						m_fpDelta = 0;
					}
				};

				struct SimInfo_s
				{
					Float_t		m_fpFrameRate;

					Timer_s		m_stTimers[TimerTypes::MAX_TIMERS];
				};

			public:
				static Core &CreateInstance(const char *cfgFileName, int argc, char * const argv[]);
				static void ReleaseInstance();
				static Core &GetInstance();

				inline void SetFrameRate(Float_t rate);

				inline const SimInfo_s &GetSimInfo() const;
				inline const Timer_s &GetGameTimer() const;
				inline const Timer_s &GetUiTimer() const;

				void RegisterCommands(Shell::IContext &context);

				void PauseTimer(TimerTypes_t timer);
				void UnpauseTimer(TimerTypes_t timer);
				void ToggleTimerPause(TimerTypes_t timer);

				void ResetTimer(TimerTypes_t timer);

				void AddModule(Module &module, UInt32_t priority = ModulePriorities::NORMAL);
				void RemoveModule(Module &module);

				/** 
					Starts the main loop, should be called by main, only once

					Only returns when main loop stops, so after calling this, you are expected
					to have added a module capable of stopping the loop somehow
				*/
				void StartMainLoop();

				//Stops the engine
				void StopMainLoop();

			private:					
				Core(const char *cfgFileName, int argc, char * const argv[]);
				~Core();

				inline Float_t GetUpdateTime(void);
				inline Float_t GetMinFrameTime(void);			

				void CmdTime(const Shell::StringVector_t &args, Shell::Context &);
				void CmdToggleTimerPause(const Shell::StringVector_t &args, Shell::Context &);
				void CmdListModules(const Shell::StringVector_t &args, Shell::Context &);
				void CmdQuit(const Shell::StringVector_t &, Shell::Context &);

				///Called automatically by MainLoop
				void Update(Float_t seconds, Float_t delta);
				void FixedUpdate(Float_t seconds);

			private:
				SimInfo_s	m_stSimInfo;

				ModuleManager	m_clModule;

				Shell::Command	m_cmdTime;
				Shell::Command	m_cmdToggleTimerPause;
				Shell::Command	m_cmdListModules;
				Shell::Command	m_cmdQuit;

				Shell::Variable	m_varFixedTime;
				Shell::Variable	m_varEngineFPS;
				Shell::Variable	m_varMinFrameTime;

				System::Timer	m_clTimer;

				bool			m_fLaunchedBoot;
				bool			m_fStopMainLoop;									
		};

		inline const Core::SimInfo_s &Core::GetSimInfo() const
		{
			return(m_stSimInfo);
		}

		inline const Core::Timer_s &Core::GetGameTimer() const
		{
			return m_stSimInfo.m_stTimers[TimerTypes::GAME];
		}

		inline const Core::Timer_s &Core::GetUiTimer() const
		{
			return m_stSimInfo.m_stTimers[TimerTypes::UI];
		}

		inline void Core::SetFrameRate(Float_t rate)
		{
			m_stSimInfo.m_fpFrameRate = rate;
		}
	}
}

#endif
