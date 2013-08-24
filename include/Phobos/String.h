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

#include "Phobos/Defs.h"
#include "Phobos/BaseAPI.h"
#include "Phobos/Types.h"

#include <boost/utility/string_ref.hpp>

namespace Phobos
{
	typedef std::string String_t;
	typedef boost::string_ref StringRef_t;

	enum StringTrimModes_e
	{
		STRING_TRIM_LEFT = 0x01,
		STRING_TRIM_RIGHT = 0x02,
		STRING_TRIM_BOTH = STRING_TRIM_LEFT | STRING_TRIM_RIGHT
	};

	PH_BASE_API std::size_t StringReplaceAll(String_t &str, char search, char replace);
	PH_BASE_API void StringReplaceAll(String_t &out, const String_t &src, char search, char replace);	

	PH_BASE_API bool StringToBoolean(const String_t &a);	

	PH_BASE_API void StringTrim(String_t &str, int mode);
	PH_BASE_API bool StringIsBlank(const String_t &str);
	PH_BASE_API bool StringSplitBy(String_t &out, const String_t &src, char ch, size_t pos, size_t *outPos);
}

#endif
