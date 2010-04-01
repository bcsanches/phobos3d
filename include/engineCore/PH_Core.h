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
			void AddModuleToDestroyList(CoreModulePtr_t module);
						
			void CreateDefaultCmds(Context_c &context);

			void PauseTimer(CoreTimerTypes_e timer);
			void UnpauseTimer(CoreTimerTypes_e timer);
			void ToggleTimerPause(CoreTimerTypes_e timer);

			void ResetTimer(CoreTimerTypes_e timer);

		private:			
			Core_c(const String_c &name);

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

				inline bool operator<(const ModuleInfo_s &rhs)
				{
					return u32Priority < rhs.u32Priority;
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
