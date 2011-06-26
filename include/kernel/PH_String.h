/*
Phobos 3d
January 2010
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
#ifndef PH_STRING_H
#define PH_STRING_H

#include <string>
#include <sstream>

#include "PH_Defs.h"
#include "PH_KernelAPI.h"
#include "PH_Types.h"

namespace Phobos
{
	typedef std::string String_c;

	enum StringTrimModes_e
	{
		STRING_TRIM_LEFT = 0x01,
		STRING_TRIM_RIGHT = 0x02,
		STRING_TRIM_BOTH = STRING_TRIM_LEFT | STRING_TRIM_RIGHT
	};

	PH_KERNEL_API std::size_t StringReplaceAll(String_c &str, char search, char replace);
	PH_KERNEL_API void StringReplaceAll(String_c &out, const String_c &src, char search, char replace);

	PH_KERNEL_API int StringToInt(const String_c &str);
	PH_KERNEL_API Float_t StringToFloat(const String_c &str);

	PH_KERNEL_API bool StringToBoolean(const String_c &a);

	template <typename T>
	inline String_c NumberToString(T value)
	{
		std::stringstream stream;
		stream << value;

		return stream.str();
	}

	inline String_c PointerToString(const void *value)
	{
		std::stringstream stream;
		stream << reinterpret_cast<const size_t>(value);

		return stream.str();
	}

	PH_KERNEL_API void StringTrim(String_c &str, int mode);
	PH_KERNEL_API bool StringIsBlank(const String_c &str);
	PH_KERNEL_API bool StringSplitBy(String_c &out, const String_c &src, char ch, size_t pos, size_t *outPos);
}

#endif
