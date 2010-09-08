/*
Phobos 3d
  September 2010

  Copyright (C) 2005-2010 Bruno Crivelari Sanches

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Bruno Crivelari Sanches bcsanches@gmail.com
*/

#include "PH_Dictionary.h"

#include <PH_Parser.h>

#include "PH_DictionaryUtils.h"

namespace Phobos
{
	DictionaryPtr_t Dictionary_c::Create(const String_c &name)
	{
		return new Dictionary_c(name);
	}
		
	Dictionary_c::Dictionary_c(const String_c &name):
		Node_c(name)
	{
		//empty
	}
	
	Dictionary_c::~Dictionary_c()
	{
		//empty
	}

	void Dictionary_c::Load(Parser_c &parser)
	{
		String_c tokenValue;
		ParserTokens_e token = parser.GetToken(&tokenValue);

		if(token != TOKEN_OPEN_BRACE)
			PH_RaiseDictionaryParseException(parser, TOKEN_OPEN_BRACE, token, tokenValue, "Dictionary_c::Load");

		for(;;)
		{
			token = parser.GetToken(&tokenValue);
			if(token == TOKEN_CLOSE_BRACE)
				return;

			if(token != TOKEN_ID)
				PH_RaiseDictionaryParseException(parser, TOKEN_ID, token, tokenValue, "Dictionary_c::Load");

			String_c value;
			token = parser.GetToken(&value);
			if(token != TOKEN_EQUAL)
				PH_RaiseDictionaryParseException(parser, TOKEN_EQUAL, token, tokenValue, "Dictionary_c::Load");

			token = parser.GetToken(&value);
			switch(token)
			{
				case TOKEN_ID:
				case TOKEN_NUMBER:
				case TOKEN_STRING:					
					mapValues[tokenValue] = value;
					break;

				default:
					PH_RaiseDictionaryParseException(parser, TOKEN_STRING, token, tokenValue, "Dictionary_c::Load");
					break;
			}
		}
	}
}
