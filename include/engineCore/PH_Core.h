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

#include "PH_CoreModuleManager.h"
#include "PH_CoreTimer.h"
#include "PH_EngineCoreAPI.h"

namespace Phobos
{	
	class Core;

	PH_DECLARE_NODE_PTR(Core);	

	struct CoreSimInfo_s
	{
		Float_t		m_fpFrameRate;

		CoreTimer_s	m_stTimers[CORE_MAX_TIMERS];
	};


	class PH_ENGINE_CORE_API Core: public CoreModuleManager
	{
		public:
			static Core &CreateInstance();
			static void ReleaseInstance();
			static Core &GetInstance();

			Core(const String_t &name);
			~Core();
			
			void Shutdown(void);

			inline void SetFrameRate(Float_t rate);

			inline const CoreSimInfo_s &GetSimInfo() const;
			inline const CoreTimer_s &GetGameTimer() const;
			inline const CoreTimer_s &GetUiTimer() const;

			void RegisterCommands(Shell::IContext &context);

			void PauseTimer(CoreTimerTypes_e timer);
			void UnpauseTimer(CoreTimerTypes_e timer);
			void ToggleTimerPause(CoreTimerTypes_e timer);

			void ResetTimer(CoreTimerTypes_e timer);

			///Starts the main loop, should be called by main, only once
			void MainLoop();

			///Called automatically by MainLoop
			void Update(Float_t seconds, Float_t delta);
			void FixedUpdate(Float_t seconds);

		private:					
			inline Float_t GetUpdateTime(void);
			inline Float_t GetMinFrameTime(void);			

			void CmdTime(const Shell::StringVector_t &args, Shell::Context &);
			void CmdToggleTimerPause(const Shell::StringVector_t &args, Shell::Context &);
			void CmdListModules(const Shell::StringVector_t &args, Shell::Context &);
			void CmdQuit(const Shell::StringVector_t &, Shell::Context &);

		private:			
			static const String_t DEFAULT_NAME;
			static CorePtr_t ipInstance_gl;

			CoreSimInfo_s	m_stSimInfo;

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

	inline const CoreSimInfo_s &Core::GetSimInfo() const
	{
		return(m_stSimInfo);
	}

	inline const CoreTimer_s &Core::GetGameTimer() const
	{
		return m_stSimInfo.m_stTimers[CORE_GAME_TIMER];
	}

	inline const CoreTimer_s &Core::GetUiTimer() const
	{
		return m_stSimInfo.m_stTimers[CORE_UI_TIMER];
	}

	inline void Core::SetFrameRate(Float_t rate)
	{
		m_stSimInfo.m_fpFrameRate = rate;
	}
}

#endif
