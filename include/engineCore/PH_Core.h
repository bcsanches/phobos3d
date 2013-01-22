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

#include <PH_ContextCmd.h>
#include <PH_ContextVar.h>

#include <Phobos/System/Timer.h>

#include "PH_CoreModuleManager.h"
#include "PH_CoreTimer.h"
#include "PH_EngineCoreAPI.h"

namespace Phobos
{
	class IContext_c;
	class Core_c;

	PH_DECLARE_NODE_PTR(Core);	

	struct CoreSimInfo_s
	{
		Float_t		fpFrameRate;

		CoreTimer_s	stTimers[CORE_MAX_TIMERS];
	};


	class PH_ENGINE_CORE_API Core_c: public CoreModuleManager_c
	{
		public:
			static Core_c &CreateInstance();
			static void ReleaseInstance();
			static Core_c &GetInstance();

			Core_c(const String_c &name);
			~Core_c();
			
			void Shutdown(void);

			inline void SetFrameRate(Float_t rate);

			inline const CoreSimInfo_s &GetSimInfo() const;
			inline const CoreTimer_s &GetGameTimer() const;
			inline const CoreTimer_s &GetUiTimer() const;

			void RegisterCommands(IContext_c &context);

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

			void CmdTime(const StringVector_t &args, Context_c &);
			void CmdToggleTimerPause(const StringVector_t &args, Context_c &);
			void CmdListModules(const StringVector_t &args, Context_c &);
			void CmdQuit(const StringVector_t &, Context_c &);

		private:			
			static const String_c DEFAULT_NAME;
			static CorePtr_t ipInstance_gl;

			CoreSimInfo_s	stSimInfo;

			ContextCmd_c	cmdTime;
			ContextCmd_c	cmdToggleTimerPause;
			ContextCmd_c	cmdListModules;
			ContextCmd_c	cmdQuit;

			ContextVar_c	varFixedTime;
			ContextVar_c	varEngineFPS;
			ContextVar_c	varMinFrameTime;

			System::Timer_c	clTimer;

			bool			fLaunchedBoot;
			bool			fStopMainLoop;									
	};

	inline const CoreSimInfo_s &Core_c::GetSimInfo() const
	{
		return(stSimInfo);
	}

	inline const CoreTimer_s &Core_c::GetGameTimer() const
	{
		return stSimInfo.stTimers[CORE_GAME_TIMER];
	}

	inline const CoreTimer_s &Core_c::GetUiTimer() const
	{
		return stSimInfo.stTimers[CORE_UI_TIMER];
	}

	inline void Core_c::SetFrameRate(Float_t rate)
	{
		stSimInfo.fpFrameRate = rate;
	}
}

#endif
