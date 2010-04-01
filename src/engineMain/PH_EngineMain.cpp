#include <sstream>

#include <OgreTimer.h>

#include <PH_ContextVar.h>
#include <PH_Core.h>
#include <PH_Kernel.h>

#define UPDATE_TIME (1.0f / 60.0f)
#define MIN_TIME (10.0f / 1000.0f)

namespace Phobos
{
	class SystemInfo_c
	{
		public:
			inline SystemInfo_c(void):
				varFixedTime("dvFixedTime", "0"),
				varEngineFPS("dvEngineFPS", "60"),
				varMinFrameTime("dvMinFrameTime", "0.01"),
				fStop(false)
			{
			}			

			ContextVar_c	varFixedTime;
			ContextVar_c	varEngineFPS;
			ContextVar_c	varMinFrameTime;
			Ogre::Timer		clTimer;
			bool			fStop;
	};

	static SystemInfo_c clSystemInfo_gl;

	static void InitPhobos()
	{
		Kernel_c::CreateInstance("phobos.log");
		Core_c::CreateInstance();
	}

	static void FinalizePhobos()
	{
		Core_c::ReleaseInstance();
		Kernel_c::DestroyInstance();
	}

	inline Float_t ConvertMSecToSeconds(UInt_t msec)
	{
		return((Float_t) msec / 1000.0f);
	}

	static inline Float_t GetUpdateTime(void)
	{
		const Float_t updateTime = clSystemInfo_gl.varEngineFPS.GetFloat();

		if(updateTime > 0)
			return(1.0f / updateTime);
		else
		{
			using namespace std;
			stringstream stream;
			stream << "[MainLoop] Warning: Invalid update time: " << updateTime << ", must be > 0" << endl;
			Kernel_c::GetInstance().LogMessage(stream.str());
			clSystemInfo_gl.varEngineFPS.SetValue("60");

			return(1.0f / UPDATE_TIME);
		}
	}

	static inline Float_t GetMinFrameTime(void)
	{
		Float_t minFrameTime = clSystemInfo_gl.varMinFrameTime.GetFloat();

		if(minFrameTime > 0.05)
		{
			using namespace std;
			stringstream stream;
			stream << "[MainLoop] Warning: Invalid minFrameTime: " << minFrameTime << ", must be < 0.05" << endl;
			Kernel_c::GetInstance().LogMessage(stream.str());			
			clSystemInfo_gl.varMinFrameTime.SetValue("0.01");

			minFrameTime = MIN_TIME;
		}

		return minFrameTime;
	}

	/**

		The engine main loop

	*/
	static void MainLoop(void)
	{
		Float_t			executionTime = 0;
		unsigned int	lastTicks;					
		Float_t			updateTime = GetUpdateTime();

		CorePtr_t core = Core_c::GetInstance();					

		lastTicks = clSystemInfo_gl.clTimer.getMilliseconds();
		do 
		{		
			core->SetFrameRate(updateTime);

			UInt_t ticks = clSystemInfo_gl.clTimer.getMilliseconds();

			Float_t lastFrameTime = ConvertMSecToSeconds(ticks - lastTicks);

			if(clSystemInfo_gl.varFixedTime.GetBoolean())
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

		} while(!clSystemInfo_gl.fStop);
	}
}

int main(int, char **)
{
	using namespace Phobos;

	InitPhobos();

	MainLoop();

	FinalizePhobos();

	return 0;
}
