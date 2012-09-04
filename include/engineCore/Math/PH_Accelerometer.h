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

#ifndef PH_MATH_ACCELEROMETER_H
#define PH_MATH_ACCELEROMETER_H

#include "PH_Interpolator.h"
#include "PH_Functions.h"

namespace Phobos
{
	namespace Math
	{
		class Accelerometer_c
		{
			public:
				inline Accelerometer_c(Float_t accelTime);

				inline void Reset();

				inline void Update(Float_t ticks);

				inline void SetButtonState(Float_t buttonState);

				inline void SetAccelerationTime(Float_t value);
				inline void SetRestBoost(Float_t value);

				inline Float_t GetValue() const;

			private:
				Float_t fpAccelTime;
				Float_t fpButtonState;
				Float_t fpRestBoost;

				LinearInterpolator_c<Float_t> clInterpolator;
		};

		inline Accelerometer_c::Accelerometer_c(Float_t accelTime):
			fpAccelTime(accelTime),
			fpButtonState(0.0f),
			clInterpolator(0, 0, 0, 0),
			fpRestBoost(1)
		{
		}

		inline void Accelerometer_c::SetRestBoost(Float_t v)
		{
			fpRestBoost = v;
		}

		inline void Accelerometer_c::SetAccelerationTime(Float_t v)
		{
			fpAccelTime = v;
		}

		inline void Accelerometer_c::Reset()
		{
			clInterpolator.Start(0, 0, 0, 0);
		}

		inline void Accelerometer_c::SetButtonState(Float_t buttonState)
		{
			if(fpButtonState == buttonState)
				return;

			Float_t currentValue = clInterpolator.GetCurrentValue();
			
			Float_t boost = 1;
			if(buttonState == 0)
				boost = fpRestBoost;


			Float_t buttonDelta = (buttonState > currentValue) ? buttonState - currentValue : currentValue - buttonState;
			if(buttonDelta == 0)
			{
				clInterpolator.Start(clInterpolator.GetCurrentTime(), 0, currentValue, currentValue);
			}
			else
			{
				clInterpolator.Start(clInterpolator.GetCurrentTime(), buttonDelta * (fpAccelTime * boost), currentValue, buttonState);
			}
					
			fpButtonState = buttonState;
		}

		inline void Accelerometer_c::Update(Float_t ticks)
		{
			clInterpolator.Update(ticks);
		}

		inline Float_t Accelerometer_c::GetValue() const
		{
			return clInterpolator.GetCurrentValue();
		}
	}
}

#endif
