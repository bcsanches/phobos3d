#include <sstream>

#include <boost/thread.hpp>

#include <OgreTimer.h>

#include <PH_ContextVar.h>
#include <PH_Core.h>
#include <PH_Kernel.h>

#define UPDATE_TIME (1.0f / 60.0f)
#define MIN_TIME (10.0f / 1000.0f)

namespace Phobos
{
	class EngineMain_c
	{
		public:
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
	};

	EngineMain_c::EngineMain_c():
		varFixedTime("dvFixedTime", "0"),
		varEngineFPS("dvEngineFPS", "60"),
		varMinFrameTime("dvMinFrameTime", "0.01"),
		fStop(false)
	{
		Kernel_c::CreateInstance("phobos.log");
		Core_c::CreateInstance();
	}

	EngineMain_c::~EngineMain_c()
	{
		Core_c::ReleaseInstance();
		Kernel_c::DestroyInstance();
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

	engine.MainLoop();	

	return 0;
}
