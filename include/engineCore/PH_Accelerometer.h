/*
Phobos 3d
September 2011
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

#ifndef PH_ACCELEROMETER_H
#define PH_ACCELEROMETER_H

#include "PH_Interpolator.h"
#include "PH_MathFunctions.h"

namespace Phobos
{
	class Accelerometer_c
	{
		public:
			Accelerometer_c(Float_t accelTime);

			void Start(Float_t startTime);

			void Update(Float_t ticks);

			void SetButtonState(Float_t buttonState);

			Float_t GetValue() const;

		private:
			Float_t fpAccelTime;
			Float_t fpButtonState;

			LinearInterpolator_c<Float_t> clInterpolator;
	};

	Accelerometer_c::Accelerometer_c(Float_t accelTime):
		fpAccelTime(accelTime),
		fpButtonState(0.0f),
		clInterpolator(0, 0, 0, 0)
	{
	}

	void Accelerometer_c::Start(Float_t startTime)
	{
		clInterpolator.Start(startTime, 0, 0, 0);
	}

	void Accelerometer_c::SetButtonState(Float_t buttonState)
	{
		if(fpButtonState == buttonState)
			return;

		Float_t currentValue = clInterpolator.GetCurrentValue();

		Float_t buttonDelta = (buttonState > currentValue) ? buttonState - currentValue : currentValue - buttonState;
		if(buttonDelta == 0)
		{
			clInterpolator.Start(clInterpolator.GetCurrentTime(), 0, currentValue, currentValue);
		}
		else
		{
			clInterpolator.Start(clInterpolator.GetCurrentTime(), buttonDelta * fpAccelTime, currentValue, buttonState);
		}
				
		fpButtonState = buttonState;
	}

	void Accelerometer_c::Update(Float_t ticks)
	{
		clInterpolator.Update(ticks);
	}

	Float_t Accelerometer_c::GetValue() const
	{
		return clInterpolator.GetCurrentValue();
	}
}

#endif
