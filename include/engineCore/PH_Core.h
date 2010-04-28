/*
Phobos 3d
  April 2010

  Copyright (C) 2005-2010 Bruno Crivelari Sanches

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Bruno Crivelari Sanches bcsanches@gmail.com
*/

#ifndef PH_CORE_H
#define PH_CORE_H

#include <set>
#include <vector>

#include <PH_ContextCmd.h>
#include <PH_Node.h>

#include "PH_CoreModule.h"
#include "PH_CoreTimer.h"
#include "PH_EngineCoreAPI.h"

namespace Phobos
{
	class Context_c;
	class Core_c;	

	typedef boost::intrusive_ptr<Core_c> CorePtr_t;

	enum CoreModulePriorities_e
	{
		LOWEST_PRIORITY		= 0,
		LOW_PRIORITY		= 0xFF,
		MEDIUM_PRIORITY		= 0xFFF,
		NORMAL_PRIORITY		= 0xFFFF,
		HIGH_PRIORITY		= 0xFFFFF,
		HIGHEST_PRIORITY	= 0xFFFFFFFF
	};

	struct CoreSimInfo_s
	{
		Float_t		fpFrameRate;

		CoreTimer_s	stTimers[CORE_MAX_TIMERS];
	};


	class PH_ENGINE_CORE_API Core_c: public Node_c
	{
		public:
			static CorePtr_t CreateInstance();
			static void ReleaseInstance();
			static CorePtr_t GetInstance();

			void Update(Float_t seconds, Float_t delta);
			void FixedUpdate(Float_t seconds);
			void Shutdown(void);

			inline void SetFrameRate(Float_t rate);

			inline const CoreSimInfo_s &GetSimInfo(void) const;
			
			void AddModule(CoreModulePtr_t module, UInt32_t priority = NORMAL_PRIORITY);
			void AddModuleToDestroyList(CoreModule_c &module);
						
			void CreateDefaultCmds(Context_c &context);

			void PauseTimer(CoreTimerTypes_e timer);
			void UnpauseTimer(CoreTimerTypes_e timer);
			void ToggleTimerPause(CoreTimerTypes_e timer);

			void ResetTimer(CoreTimerTypes_e timer);

		private:			
			Core_c(const String_c &name);

			void CallCoreModuleProc(CoreModuleProc_t proc);

			void UpdateDestroyList();

			void CmdTime(const StringVector_t &args, Context_c &);
			void CmdToggleTimerPause(const StringVector_t &args, Context_c &);
			void CmdListModules(const StringVector_t &args, Context_c &);

		private:
			struct ModuleInfo_s
			{
				UInt32_t u32Priority;
				CoreModulePtr_t ipModule;

				inline ModuleInfo_s(CoreModulePtr_t module, UInt32_t priority):
					ipModule(module),
					u32Priority(priority)
				{
				}

				inline ModuleInfo_s(const ModuleInfo_s &rhs):
					ipModule(rhs.ipModule),
					u32Priority(rhs.u32Priority)
				{
				}

				inline bool operator<(const ModuleInfo_s &rhs) const
				{
					return rhs.u32Priority < u32Priority;
				}

				inline bool operator==(const CoreModule_c &module) const
				{
					return ipModule.get() == &module;
				}

				inline bool operator==(const CoreModulePtr_t module) const
				{
					return ipModule == module;
				}
			};

			static const String_c DEFAULT_NAME;
			static CorePtr_t ipInstance_gl;

			typedef std::vector<ModuleInfo_s>	ModulesVector_t;
			ModulesVector_t						vecModules;
			CoreSimInfo_s						stSimInfo;

			typedef std::set<CoreModulePtr_t>	ModulesSet_t;
			ModulesSet_t						setModulesToDestroy;

			ContextCmd_c	cmdTime;
			ContextCmd_c	cmdToggleTimerPause;
			ContextCmd_c	cmdListModules;
	};

	inline const CoreSimInfo_s &Core_c::GetSimInfo(void) const
	{
		return(stSimInfo);
	}

	inline void Core_c::SetFrameRate(Float_t rate)
	{
		stSimInfo.fpFrameRate = rate;
	}
}

#endif
