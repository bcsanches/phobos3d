/*
Phobos 3d
April 2010
Copyright (c) 2005-2011 Bruno Sanches  http://code.google.com/p/phobos3d

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef PH_CORE_TIMER_H
#define PH_CORE_TIMER_H

namespace Phobos
{
	enum CoreTimerTypes_e
	{
		CORE_SYS_TIMER,
		CORE_GAME_TIMER,
		CORE_UI_TIMER,
		

		CORE_MAX_TIMERS
	};

	enum CoreTimerFlags_e
	{
		TIMER_PAUSED = 0x01,
	};

	struct CoreTimer_s
	{
		//* Number of ticks occurred since started.
		Float_t	fpTotalTicks;

		//* Number of ticks for simulation frame
		Float_t	fpFrameTime;

		//* number of ticks for the rendering frame
		Float_t	fpRenderFrameTime;


		Float_t	fpTotalRenderFrameTime;

		//*number of frames occurred
		unsigned int uFrameCount;

		//*delta used for rendering interpolation
		Float_t	fpDelta;

		UInt32_t u32Flags;

		inline bool IsPaused(void) const
		{
			return(u32Flags & TIMER_PAUSED);
		}

		inline void Pause(void)
		{
			u32Flags |= TIMER_PAUSED;
		}

		inline void Unpause(void)
		{
			u32Flags &= ~TIMER_PAUSED;
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
			fpTotalTicks = 0;
			fpFrameTime = 0;
			fpRenderFrameTime = 0;
			fpTotalRenderFrameTime = 0;
			uFrameCount = 0;
			fpDelta = 0;
		}
	};
}

#endif
