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
		class Accelerometer
		{
			public:
				inline Accelerometer(Float_t accelTime);

				inline void Reset();

				inline void Update(Float_t ticks);

				inline void SetButtonState(Float_t buttonState);

				inline void SetAccelerationTime(Float_t value);
				inline void SetRestBoost(Float_t value);

				inline Float_t GetValue() const;

			private:
				Float_t m_fpAccelTime;
				Float_t m_fpButtonState;
				Float_t m_fpRestBoost;

				LinearInterpolator<Float_t> m_clInterpolator;
		};

		inline Accelerometer::Accelerometer(Float_t accelTime):
			m_fpAccelTime(accelTime),
			m_fpButtonState(0.0f),
			m_clInterpolator(0, 0, 0, 0),
			m_fpRestBoost(1)
		{
		}

		inline void Accelerometer::SetRestBoost(Float_t v)
		{
			m_fpRestBoost = v;
		}

		inline void Accelerometer::SetAccelerationTime(Float_t v)
		{
			m_fpAccelTime = v;
		}

		inline void Accelerometer::Reset()
		{
			m_clInterpolator.Start(0, 0, 0, 0);
		}

		inline void Accelerometer::SetButtonState(Float_t buttonState)
		{
			if(m_fpButtonState == buttonState)
				return;

			Float_t currentValue = m_clInterpolator.GetCurrentValue();
			
			Float_t boost = 1;
			if(buttonState == 0)
				boost = m_fpRestBoost;


			Float_t buttonDelta = (buttonState > currentValue) ? buttonState - currentValue : currentValue - buttonState;
			if(buttonDelta == 0)
			{
				m_clInterpolator.Start(m_clInterpolator.GetCurrentTime(), 0, currentValue, currentValue);
			}
			else
			{
				m_clInterpolator.Start(m_clInterpolator.GetCurrentTime(), buttonDelta * (m_fpAccelTime * boost), currentValue, buttonState);
			}
					
			m_fpButtonState = buttonState;
		}

		inline void Accelerometer::Update(Float_t ticks)
		{
			m_clInterpolator.Update(ticks);
		}

		inline Float_t Accelerometer::GetValue() const
		{
			return m_clInterpolator.GetCurrentValue();
		}
	}
}

#endif
