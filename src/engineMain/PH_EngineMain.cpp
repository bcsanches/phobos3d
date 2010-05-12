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

#include <sstream>

#include <boost/foreach.hpp>
#include <boost/thread.hpp>

#include <OgreTimer.h>

#include <PH_Console.h>
#include <PH_ContextVar.h>
#include <PH_Core.h>
#include <PH_EventManagerModule.h>
#include <PH_Kernel.h>
#include <PH_Render.h>

#define UPDATE_TIME (1.0f / 60.0f)
#define MIN_TIME (10.0f / 1000.0f)

namespace Phobos
{
	class EngineMain_c
	{
		public:
			typedef void (*ReleaseProc_t)();

			EngineMain_c();
			~EngineMain_c();

			inline Float_t ConvertMSecToSeconds(UInt_t msec);
			inline Float_t GetUpdateTime(void);
			inline Float_t GetMinFrameTime(void);

			void MainLoop(void);

		private:
			ContextVar_c	varFixedTime;
			ContextVar_c	varEngineFPS;
			ContextVar_c	varMinFrameTime;
			Ogre::Timer		clTimer;
			bool			fStop;

			std::vector<ReleaseProc_t> vecSingletons;
	};

	EngineMain_c::EngineMain_c():
		varFixedTime("dvFixedTime", "0"),
		varEngineFPS("dvEngineFPS", "60"),
		varMinFrameTime("dvMinFrameTime", "0.01"),
		fStop(false)
	{
		Kernel_c::CreateInstance("phobos.log");
		CorePtr_t core = Core_c::CreateInstance();	

		EventManagerModulePtr_t eventManager = EventManagerModule_c::CreateInstance();
		vecSingletons.push_back(EventManagerModule_c::ReleaseInstance);
		core->AddModule(eventManager);

		ConsolePtr_t console = Console_c::CreateInstance();
		vecSingletons.push_back(Console_c::ReleaseInstance);
		core->AddModule(console);

		RenderPtr_t render = Render_c::CreateInstance();
		vecSingletons.push_back(Render_c::ReleaseInstance);
		core->AddModule(render);

		core->LaunchBootModule();
	}

	EngineMain_c::~EngineMain_c()
	{
		BOOST_FOREACH(ReleaseProc_t proc, vecSingletons)
		{
			proc();
		}

		Core_c::ReleaseInstance();
		Kernel_c::ReleaseInstance();
	}

	inline Float_t EngineMain_c::ConvertMSecToSeconds(UInt_t msec)
	{
		return((Float_t) msec / 1000.0f);
	}

	inline Float_t EngineMain_c::GetUpdateTime(void)
	{
		const Float_t updateTime = varEngineFPS.GetFloat();

		if(updateTime > 0)
			return(1.0f / updateTime);
		else
		{
			using namespace std;
			stringstream stream;
			stream << "[MainLoop] Warning: Invalid update time: " << updateTime << ", must be > 0" << endl;
			Kernel_c::GetInstance().LogMessage(stream.str());
			varEngineFPS.SetValue("60");

			return(1.0f / UPDATE_TIME);
		}
	}

	inline Float_t EngineMain_c::GetMinFrameTime(void)
	{
		Float_t minFrameTime = varMinFrameTime.GetFloat();

		if(minFrameTime > 0.05)
		{
			using namespace std;
			stringstream stream;
			stream << "[MainLoop] Warning: Invalid minFrameTime: " << minFrameTime << ", must be < 0.05" << endl;
			Kernel_c::GetInstance().LogMessage(stream.str());			
			varMinFrameTime.SetValue("0.01");

			minFrameTime = MIN_TIME;
		}

		return minFrameTime;
	}

	/**

		The engine main loop

	*/
	void EngineMain_c::MainLoop(void)
	{
		Float_t			executionTime = 0;
		unsigned int	lastTicks;					
		Float_t			updateTime = GetUpdateTime();

		CorePtr_t core = Core_c::GetInstance();					

		lastTicks = clTimer.getMilliseconds();
		do 
		{		
			core->SetFrameRate(updateTime);

			UInt_t ticks = clTimer.getMilliseconds();

			Float_t lastFrameTime = ConvertMSecToSeconds(ticks - lastTicks);

			if(varFixedTime.GetBoolean())
				lastFrameTime = updateTime;
			
			if(lastFrameTime < GetMinFrameTime())
			{
				boost::this_thread::sleep(boost::posix_time::milliseconds(1));
				continue;
			}

			lastTicks = ticks;

			//how long the simulation can run?
			executionTime += lastFrameTime;
			Float_t totalFrameTime = lastFrameTime;

			#ifdef IM_DEBUG
				//this happens on debug mode while stopped on break points
				if(executionTime > 20)
					executionTime = updateTime;
			#endif		

			//update the game on fixed time steps
			while(executionTime >= updateTime)
			{			
				//fixed Update
				core->FixedUpdate(updateTime);

				//totalFrameTime += time;

				//printf("%f\n", executionTime);
				executionTime -= updateTime;
			}
			Float_t delta = (Float_t) executionTime / (Float_t) updateTime;		
			//printf("%f\n", delta);
			//Now update other modules as fast as we can
			core->Update(totalFrameTime, delta);

			//update it after frame
			updateTime = GetUpdateTime();

		} while(!fStop);
	}
}

int main(int, char **)
{	
	Phobos::EngineMain_c engine;

	try
	{
		engine.MainLoop();		
	}
	catch(std::exception &e)
	{
		std::stringstream stream;
		stream << "main: Unhandled excetion: ";
		stream << e.what();
		Phobos::Kernel_c::GetInstance().LogMessage(stream.str());

		exit(EXIT_FAILURE);
	}

	return 0;
}
