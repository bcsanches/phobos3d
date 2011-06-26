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

#ifndef PH_INTERPOLATOR_H
#define PH_INTERPOLATOR_H

namespace Phobos
{
	template <typename T>
	class Interpolator_c
	{
		public:			
			typedef T &Reference_t;


		public:
			inline Interpolator_c(void)
			{
				//empty
			}

			inline Interpolator_c(const T &t):
				tCurrent(t),
				tLast(t)
			{
			}

			inline void SetValue(const T &v)
			{
				tLast = tCurrent;
				tCurrent = v;
			}

			inline void Reset(const T &v)
			{
				tLast = v;
				tCurrent = v;
			}

			inline void SetValues(const T &from, const T &to)
			{
				tLast = from;
				tCurrent = to;
			}

			inline T GetValue(Float_t alpha)
			{
				return MathInterpolate(tLast, tCurrent, alpha);
			}

			inline T & GetValue(T &out, Float_t alpha)
			{
				out = this->GetValue(alpha);
				return(out);
			}

			inline const T & GetCurrent(void) const
			{
				return(tCurrent);
			}

			inline const T & GetLast(void) const
			{
				return(tLast);
			}

		private:
			T tCurrent;
			T tLast;
	};
}

#endif
