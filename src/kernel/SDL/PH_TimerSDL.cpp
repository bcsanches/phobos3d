#include "PH_Timer.h"

#include <SDL.h>

namespace Phobos
{		
	Timer_c::Timer_c():
		uLastTime(SDL_GetTicks()),
		fpMinInterval(0)
	{
	}

	void Timer_c::Reset()
	{
		uLastTime = SDL_GetTicks();
	}

	Float_t Timer_c::Elapsed()
	{
		UInt_t ticks = SDL_GetTicks();		
		
		//UInt_t elapsed = (ticks < uLastTime) ? ticks + (0xFFFFFFFF - uLastTime) : ticks - uLastTime;

#if 0
		if(ticks < uLastTime)
			ticks = uLastTime+1;
#endif

		UInt_t elapsed = ticks - uLastTime;		

		Float_t elapsedSeconds = elapsed / 1000.0f;
		if((elapsedSeconds < fpMinInterval) || (elapsed == 0))
			return 0.0f;

		uLastTime = ticks;

		return elapsedSeconds;
	}	
}
