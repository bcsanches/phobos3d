/*
Phobos 3d
February 2012
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


#include "Phobos/System/Timer.h"

#include <SDL.h>

Phobos::System::Timer::Timer():
	m_uLastTime(SDL_GetTicks()),
	m_fpMinInterval(0)
{
}

void Phobos::System::Timer::Reset()
{
	m_uLastTime = SDL_GetTicks();
}

Phobos::Float_t Phobos::System::Timer::Elapsed()
{
	UInt_t ticks = SDL_GetTicks();		
		
	//UInt_t elapsed = (ticks < uLastTime) ? ticks + (0xFFFFFFFF - uLastTime) : ticks - uLastTime;

#if 0
	if(ticks < uLastTime)
		ticks = uLastTime+1;
#endif

	UInt_t elapsed = ticks - m_uLastTime;		

	Float_t elapsedSeconds = elapsed / 1000.0f;
	if((elapsedSeconds < m_fpMinInterval) || (elapsed == 0))
		return 0.0f;

	m_uLastTime = ticks;

	return elapsedSeconds;
}	

