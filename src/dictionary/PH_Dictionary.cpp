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

#include <PH_Exception.h>
#include <PH_Parser.h>

#include "PH_DictionaryManager.h"
#include "PH_DictionaryUtils.h"
#include <stdio.h>

namespace Phobos
{
	DictionaryPtr_t Dictionary_c::Create(const String_c &name)
	{
		return new Dictionary_c(name);
	}

	Dictionary_c::Dictionary_c(const String_c &name):
		Node_c(name),
		pclInherit(NULL)
	{
		//empty
	}

	Dictionary_c::~Dictionary_c()
	{
		//empty
	}

	void Dictionary_c::AddValue(const String_c &key, const String_c &value)
	{
		mapValues[key] = value;

		if(key.compare("inherit") == 0)
			strInherit = value;
		if(key.compare("baseHive") == 0)
			strBaseHive = value;
	}

	void Dictionary_c::Load(Parser_c &parser)
	{
		String_c idName;
		String_c value;

		ParserTokens_e token = parser.GetToken(&value);

		if(token != TOKEN_OPEN_BRACE)
			PH_RaiseDictionaryParseException(parser, TOKEN_OPEN_BRACE, token, value, "Dictionary_c::Load");

		for(;;)
		{
			token = parser.GetToken(&value);
			if(token == TOKEN_CLOSE_BRACE)
				break;

			idName.clear();
			//Accepts name in the format:
			//model=
			//RenderCompoenent.model=
			for(;;)
			{
				if(token != TOKEN_ID)
					PH_RaiseDictionaryParseException(parser, TOKEN_ID, token, value, "Dictionary_c::Load");
			
				idName.append(value);
				token = parser.GetToken(&value);
				if(token == TOKEN_DOT)
				{
					idName.append(value);
					token = parser.GetToken(&value);
					continue;
				}

				if(token == TOKEN_EQUAL)
					break;

				PH_RaiseDictionaryParseException(parser, TOKEN_EQUAL, token, value, "Dictionary_c::Load");				
			}

			token = parser.GetToken(&value);
			switch(token)
			{
				case TOKEN_ID:
				case TOKEN_NUMBER:
				case TOKEN_STRING:
					this->AddValue(idName, value);
					break;

				default:
					PH_RaiseDictionaryParseException(parser, TOKEN_STRING, token, idName, "Dictionary_c::Load");
					break;
			}

			token = parser.GetToken(&value);
			if(token != TOKEN_SEMI_COLON)
				PH_RaiseDictionaryParseException(parser, TOKEN_SEMI_COLON, token, value, "Dictionary_c::Load");
		}
	}

	const String_c &Dictionary_c::GetValue(const String_c &key) const
	{
		const String_c *foundValue = TryGetValue(this, key);
		if(!foundValue)
		{
			std::stringstream stream;
			stream << "Value " << key << " does not exists in " << this->GetName();
			PH_RAISE(OBJECT_NOT_FOUND_EXCEPTION, "Dictionary_c::GetValue", stream.str());
		}

		return *foundValue;
	}

	bool Dictionary_c::TryGetValue(const String_c &key, String_c &value) const
	{
		const String_c *foundValue = TryGetValue(this, key);
		if(!foundValue)
			return false;

		value = *foundValue;
		return true;
	}

	const String_c *Dictionary_c::TryGetValue(const String_c &key) const
	{
		return TryGetValue(this, key);
	}

	bool Dictionary_c::GetBool(const String_c &key) const
	{
		const String_c &value = this->GetValue(key);

		return value.compare("true") == 0 ? true : false;
	}

	void Dictionary_c::Get4Float(const float values[4], const String_c &key) const
	{
		const String_c &value = this->GetValue(key);

		sscanf(value.c_str(), "%f %f %f %f", &values[0], &values[1], &values[2], &values[3]);
	}

	void Dictionary_c::Get3Float(const float values[3], const String_c &key) const
	{
		const String_c &value = this->GetValue(key);

		sscanf(value.c_str(), "%f %f %f", &values[0], &values[1], &values[2]);
	}

	const Dictionary_c *Dictionary_c::GetInherited() const
	{
		if(pclInherit)
			return pclInherit;

		if(!strInherit.empty())
		{
			if(strBaseHive.empty())
				pclInherit = boost::static_pointer_cast<Dictionary_c>(this->GetParent()->GetChild(strInherit)).get();
			else
				pclInherit = DictionaryManager_c::GetInstance()->GetDictionary(strBaseHive, strInherit).get();
		}

		return pclInherit;
	}

	int Dictionary_c::GetInt(const String_c &key) const
	{
		return StringToInt(this->GetValue(key));
	}

	float Dictionary_c::GetFloat(const String_c &key) const
	{
		return StringToFloat(this->GetValue(key));
	}

	const String_c *Dictionary_c::TryGetValue(const Dictionary_c *current, const String_c &key)
	{
		do
		{
			StringMap_t::const_iterator it = current->mapValues.find(key);
			if(it != current->mapValues.end())
			{
				return &it->second;
			}

			current = current->GetInherited();
		}
		while(current);

		return NULL;
	}
}
