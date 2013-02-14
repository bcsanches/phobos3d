/*
Phobos 3d
October 2010
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

#ifndef PH_ENGINE_MATH_INTERPOLATOR_H
#define PH_ENGINE_MATH_INTERPOLATOR_H

#include <Phobos/Types.h>

namespace Phobos
{
	namespace Engine
	{
		namespace Math
		{
			template <typename T>
			class Interpolator
			{
				public:
					typedef T &Reference_t;


				public:
					inline Interpolator(void)
					{
						//empty
					}

					inline Interpolator(const T &t):
						m_tCurrent(t),
						m_tLast(t)
					{
					}

					inline void SetValue(const T &v)
					{
						m_tLast = m_tCurrent;
						m_tCurrent = v;
					}

					inline void Reset(const T &v)
					{
						m_tLast = v;
						m_tCurrent = v;
					}

					inline void SetValues(const T &from, const T &to)
					{
						m_tLast = from;
						m_tCurrent = to;
					}

					inline T GetValue(Float_t alpha)
					{
						return Interpolate(m_tLast, m_tCurrent, alpha);
					}

					inline T & GetValue(T &out, Float_t alpha)
					{
						out = this->GetValue(alpha);
						return(out);
					}

					inline const T & GetCurrent(void) const
					{
						return(m_tCurrent);
					}

					inline const T & GetLast(void) const
					{
						return(m_tLast);
					}

				private:
					T m_tCurrent;
					T m_tLast;
			};

			template <typename T>
			class LinearInterpolator
			{
				public:
					LinearInterpolator():
						m_fpStartTime(0),
						m_fpDuration(0),
						m_fpCurrentTime(0)
					{
					}

					LinearInterpolator(const float startTime, const float duration, const T &startValue, const T &endValue)			
					{
						this->Start(startTime, duration, startValue, endValue);
					}

					void Start(const float startTime, const float duration, const T &startValue, const T &endValue)
					{
						m_fpStartTime = startTime;
						m_fpDuration = duration;
						m_fpCurrentTime = m_fpStartTime;
						m_tStartValue = startValue;
						m_tEndValue = endValue;
						m_tCurrentValue = startValue;
					}

					void Update(Float_t ticks)
					{
						//Force a value update
						this->GetValue(m_fpCurrentTime + ticks);
					}

					T GetValue(Float_t time)
					{
						if(time != m_fpCurrentTime)
						{
							float delta = time - m_fpStartTime;
							if(delta <= 0)
							{
								m_tCurrentValue = m_tStartValue;
							}
							else if(delta >= m_fpDuration)
							{
								m_tCurrentValue = m_tEndValue;
							}
							else
							{
								m_tCurrentValue = m_tStartValue + (m_tEndValue - m_tStartValue) * (delta / m_fpDuration);
							}

							m_fpCurrentTime = time;
						}

						return m_tCurrentValue;
					}

					T GetCurrentValue() const
					{
						return m_tCurrentValue;
					}

					Float_t GetCurrentTime() const
					{
						return m_fpCurrentTime;
					}

				private:
					Float_t m_fpStartTime;
					Float_t m_fpDuration;
					Float_t m_fpCurrentTime;

					T		m_tStartValue;
					T		m_tEndValue;
					T		m_tCurrentValue;
			};
		}
	}
}

#endif
