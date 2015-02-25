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

#include <Phobos/System/Chrono.h>

#include "Phobos/Engine/ConsoleFwd.h"
#include "Phobos/Engine/ModuleContainer.h"
#include "Phobos/Engine/EngineAPI.h"

namespace Phobos
{	
	namespace Engine
	{		
		class PH_ENGINE_API Core
		{			
			public:
				Core(Shell::IContext &context, const char *cfgFileName, int argc, char * const argv[]);
				~Core();

				void AddModule(std::unique_ptr<Module> &&module, UInt32_t priority = ModulePriorities::NORMAL);
				void AddModule(Module &module, UInt32_t priority = ModulePriorities::NORMAL);
				void RemoveModule(Module &module);

				void InitModules();
				void FinalizeModules();

				/** 
					Starts the main loop, should be called by main, only once

					Only returns when main loop stops, so after calling this, you are expected
					to have added a module capable of stopping the loop somehow
				*/
				void RunMainLoop();

				/**
					Run a single frame for the engine

					@returns
						number > 0, one render frame has been run (logic frame may have run)
						number == 0, no logic and no render frame has been run, timeslice is too small, so wait and call later
						number < 0, stop main loop was called, so no loop run

				*/
				int RunSingleFrame();

				//Stops the engine
				void StopMainLoop();

			private:									
				inline System::Seconds GetUpdateTime(void);
				inline System::Seconds GetMinFrameTime(void);
				
				void CmdListModules(const Shell::StringVector_t &args, Shell::Context &);
				void CmdQuit(const Shell::StringVector_t &, Shell::Context &);

				///Called automatically by MainLoop
				void Update(System::Seconds seconds, Float_t delta);
				void FixedUpdate(System::Seconds seconds);

				void RegisterCommands(Shell::IContext &context);

			private:
				ModuleContainer	m_clModule;
				
				Shell::Command	m_cmdListModules;
				Shell::Command	m_cmdQuit;

				Shell::Variable	m_varFixedTime;
				Shell::Variable	m_varEngineFPS;
				Shell::Variable	m_varMinFrameTime;				

				bool			m_fLaunchedBoot;
				bool			m_fStopMainLoop;		


				System::Seconds				m_secExecutionTime;
				System::Clock::time_point	m_tpPreviousTime;
		};		
	}
}

#endif
