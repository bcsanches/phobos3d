/*
Phobos 3d
September 2010
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

#include "PH_DictionaryUtils.h"

#include <PH_Exception.h>
#include <PH_Parser.h>

namespace Phobos
{
	void PH_RaiseDictionaryParseException(const Parser_c &parser, ParserTokens_e expected, ParserTokens_e got, const String_c &token, const char *module)
	{
		std::stringstream stream;

		stream << "Expected " << Parser_c::GetTokenTypeName(expected) << " but got " << Parser_c::GetTokenTypeName(got) << " value: " << token << std::endl;
		//stream << "On line " << parser.Get

		PH_RAISE(PARSER_EXCEPTION, module, stream.str());
	}
}