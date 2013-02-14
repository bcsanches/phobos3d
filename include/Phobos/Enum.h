/*
Phobos 3d
May 2010
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

#ifndef PH_ENUM_H
#define PH_ENUM_H

#include "Phobos/String.h"

namespace Phobos
{
	template <typename T, typename TABLE, typename I = UInt_t>
	class Enum
	{
		public:
			Enum(const TABLE *table):
				pstEnumTable(table)
			{
			}

			bool TryGetValue(T &out, I v)
			{
				for(size_t i = 0;pstEnumTable[i].pstrzName; ++i)
				{
					if(pstEnumTable[i].eValue == v)
					{
						out = pstEnumTable[i].eValue;

						return(true);
					}
				}

				return(false);
			}

			bool TryGetValue(T &out, const String_t &str)
			{
				for(size_t i = 0;pstEnumTable[i].pstrzName; ++i)
				{
					if(str.compare(pstEnumTable[i].pstrzName)==0)
					{
						out = pstEnumTable[i].eValue;
						return(true);
					}
				}

				return(false);
			}

			const Char_t *GetName(const T v)
			{
				return(this->GetName((I)v));
			}

			const Char_t *TryGetName(const I v)
			{
				for(size_t i = 0;pstEnumTable[i].pstrzName; ++i)
				{
					if(v == pstEnumTable[i].eValue)
						return(pstEnumTable[i].pstrzName);
				}

				return(NULL);
			}

		private:
			const TABLE *pstEnumTable;
	};
}

#endif
