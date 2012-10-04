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

#include "PH_DictionaryHive.h"

#include <PH_Memory.h>
#include <PH_Parser.h>

#include "PH_Dictionary.h"
#include "PH_DictionaryUtils.h"

namespace Phobos
{
	DictionaryHivePtr_t DictionaryHive_c::Create(const String_c &name)
	{
		return DictionaryHivePtr_t(PH_NEW DictionaryHive_c(name));
	}

	DictionaryHive_c::DictionaryHive_c(const String_c &name):
		Node_c(name, NodeFlags::PRIVATE_CHILDREN),
		uSequence(0)
	{
		//empty
	}

	DictionaryHive_c::~DictionaryHive_c()
	{
		//empty
	}

	void DictionaryHive_c::AddDictionary(std::auto_ptr<Dictionary_c> dict)
	{
		dict->SetManaged(true);
		this->AddPrivateChild(*dict);

		dict.release();
	}

	void DictionaryHive_c::Load(Parser_c &parser)
	{
		String_c tokenValue;
		ParserTokens_e token = parser.GetToken(&tokenValue);

		String_c dictName;
		String_c inherit;
		String_c baseHive;

		if(token == TOKEN_ID)
		{
			dictName.swap(tokenValue);

			token = parser.GetToken(&tokenValue);
		}

		if(token == TOKEN_COLON)
		{
			token = parser.GetToken(&baseHive);
			if(token != TOKEN_ID)
			{
				PH_RaiseDictionaryParseException(parser, TOKEN_ID, token, tokenValue, "DictionaryHive_c::Load");
			}			

			token = parser.GetToken(&tokenValue);
			if(token == TOKEN_DOT)
			{
				token = parser.GetToken(&inherit);
				if(token != TOKEN_ID)
				{
					PH_RaiseDictionaryParseException(parser, TOKEN_ID, token, tokenValue, "DictionaryHive_c::Load");
				}				

				//grab next token (must be a {, to be handled later if nameless)
				token = parser.GetToken(&tokenValue);
			}
			else if(token != TOKEN_OPEN_BRACE)
				PH_RaiseDictionaryParseException(parser, TOKEN_OPEN_BRACE, token, tokenValue, "DictionaryHive_c::Load");
			else
			{
				inherit.swap(baseHive);
			}
		}

		if(token == TOKEN_OPEN_BRACE)
		{
			//generate name
			if(dictName.empty())
			{
				dictName = "#autoName_";
				dictName += NumberToString(uSequence++);
			}

			parser.PushToken();
		}		

		std::auto_ptr<Dictionary_c> dict(PH_NEW Dictionary_c(dictName));		

		if(!baseHive.empty())
			dict->SetBaseHive(baseHive);

		if(!inherit.empty())
			dict->SetInherited(inherit);

		dict->Load(parser);		

		this->AddDictionary(dict);		
	}

	Dictionary_c &DictionaryHive_c::GetDictionary(const String_c &name)
	{
		return static_cast<Dictionary_c &>(this->GetChild(name));
	}

	Dictionary_c *DictionaryHive_c::TryGetDictionary(const String_c &name)
	{
		return static_cast<Dictionary_c *>(this->TryGetChild(name));
	}
}