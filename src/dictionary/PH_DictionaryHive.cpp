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

#include <PH_Parser.h>

#include "PH_Dictionary.h"
#include "PH_DictionaryUtils.h"

namespace Phobos
{
	DictionaryHivePtr_t DictionaryHive_c::Create(const String_c &name)
	{
		return DictionaryHivePtr_t(new DictionaryHive_c(name));
	}

	DictionaryHive_c::DictionaryHive_c(const String_c &name):
		Node_c(name, PRIVATE_CHILDREN)
	{
		//empty
	}

	DictionaryHive_c::~DictionaryHive_c()
	{
	}

	void DictionaryHive_c::AddDictionary(DictionaryPtr_t dict)
	{
		this->AddPrivateChild(dict);
	}

	void DictionaryHive_c::Load(Parser_c &parser)
	{
		String_c tokenValue;
		ParserTokens_e token = parser.GetToken(&tokenValue);
		if(token != TOKEN_ID)
			PH_RaiseDictionaryParseException(parser, TOKEN_ID, token, tokenValue, "DictionaryHive_c::Load");

		DictionaryPtr_t dict = Dictionary_c::Create(tokenValue);
		dict->Load(parser);

		this->AddDictionary(dict);		
	}

	DictionaryPtr_t DictionaryHive_c::GetDictionary(const String_c &name)
	{
		return boost::static_pointer_cast<Dictionary_c>(this->GetChild(name));
	}

	DictionaryPtr_t DictionaryHive_c::TryGetDictionary(const String_c &name)
	{
		return boost::static_pointer_cast<Dictionary_c>(this->TryGetChild(name));
	}
}