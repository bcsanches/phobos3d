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

#include "PH_Dictionary.h"

#include <PH_Exception.h>
#include <PH_Parser.h>

#include "PH_DictionaryManager.h"
#include "PH_DictionaryUtils.h"
#include <stdio.h>

#define INHERIT_KEY "inherit"
#define BASE_HIVE_KEY "baseHive"

#define NEW_KEYWORD "new"

namespace Phobos
{
	static const char *parszStringOnlyKeys_g[] =
	{
		INHERIT_KEY,
		BASE_HIVE_KEY,
		NULL
	};

	static const char *parszKeywords_g[] = 
	{
		NEW_KEYWORD,
		NULL
	};

	DictionaryPtr_t Dictionary_c::Create(const String_c &name)
	{
		return DictionaryPtr_t(new Dictionary_c(name));
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

	void Dictionary_c::CheckInvalidKey(const String_c &key, const char *keys[], const char *message) const
	{
		for(int i = 0;keys[i]; ++i)
		{
			if(key.compare(keys[i]) == 0)
			{
				std::stringstream stream;
				stream << "Value " << key << " " << message;
				PH_RAISE(INVALID_PARAMETER_EXCEPTION, "Dictionary_c::CheckInvalidKey", stream.str());
			}
		}
	}

	void Dictionary_c::CheckForKeyword(const String_c &key) const
	{
		this->CheckInvalidKey(key, parszKeywords_g, "cannot be a key because it is a reserved keyword");		
	}

	void Dictionary_c::SetInherited(const String_c &base)
	{
		this->SetString(INHERIT_KEY, base);
	}

	void Dictionary_c::SetBaseHive(const String_c &baseHive)
	{
		this->SetString(BASE_HIVE_KEY, baseHive);
	}

	void Dictionary_c::SetString(const String_c &key, const String_c &value)
	{	
		CheckForKeyword(key);

		mapValues[key] = Value_s(value);

		if(key.compare(INHERIT_KEY) == 0)
		{			
			strInherit = value;
			pclInherit = NULL;
		}
		if(key.compare(BASE_HIVE_KEY) == 0)
		{			
			strBaseHive = value;
			pclInherit = NULL;
		}
	}

	void Dictionary_c::SetCharMatrix(const String_c &key, const String_c &data, UInt16_t numRows, UInt16_t numColumns)
	{	
		CheckForKeyword(key);
		this->CheckInvalidKey(key, parszStringOnlyKeys_g, "should be string data, not matrix");

		if(numRows * numColumns == 0)
		{
			PH_RAISE(INVALID_PARAMETER_EXCEPTION, "Dictionary_c::ParseSpecialValue", "Matrix cannot be empty");
		}

		if(numRows * numColumns != data.length())
		{
			std::stringstream stream;
			stream << "Matrix data size (" << data.length() << ") does not match width (" << numColumns << ") and height (" << numRows << ") parameters";
			PH_RAISE(INVALID_PARAMETER_EXCEPTION, "Dictionary_c::ParseSpecialValue", stream.str());
		}		

		mapValues[key] = Value_s(data, numRows, numColumns);
	}

	void Dictionary_c::ParseSpecialValue(const String_c &idName, Parser_c &parser)
	{
		String_c type;

		ParserTokens_e token;
		if((token = parser.GetToken(&type)) != TOKEN_ID)
		{
			PH_RaiseDictionaryParseException(parser, TOKEN_ID, token, type, "Dictionary_c::ParseSpecialValue");
		}

		if(type.compare("CharMatrix") == 0)
		{
			if((token = parser.GetToken(NULL)) != TOKEN_OPEN_PAREN)
			{
				PH_RaiseDictionaryParseException(parser, TOKEN_OPEN_PAREN, token, type, "Dictionary_c::ParseSpecialValue");
			}

			String_c matrix;
			String_c row;

			UInt16_t numColumns = 0;
			UInt16_t numRows = 0;

			bool first = true;
			for(;;)
			{
				token = parser.GetToken(&row);
				if(token == TOKEN_CLOSE_PAREN)
				{
					if(first)
					{
						//do not allow empty matrix
						PH_RaiseDictionaryParseException(parser, "matrix data", "closing parenthesis", "Dictionary_c::ParseSpecialValue");
					}
					
					this->SetCharMatrix(idName, matrix, numRows, numColumns);
					break;
				}
				else if(token == TOKEN_STRING)
				{
					if(first)
					{
						numColumns = row.length();

						if(numColumns == 0)
							PH_RAISE(PARSER_EXCEPTION, "Dictionary_c::ParseSpecialValue", "Matrix cannot be empty");

						first = false;
					}
					else if(numColumns != row.length())
					{
						PH_RAISE(PARSER_EXCEPTION, "Dictionary_c::ParseSpecialValue", "Matrix rows should always have the same length");
					}

					matrix.append(row);
					++numRows;
				}
				else
				{
					PH_RaiseDictionaryParseException(parser, TOKEN_STRING, token, row, "Dictionary_c::ParseSpecialValue");
				}
			}
		}	
		else
		{
			PH_RaiseDictionaryParseException(parser, " valid especial type, ie CharMatrix", type.c_str(), "Dictionary_c::ParseSpecialValue");
		}
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
					if(value.compare("new") == 0)
					{
						this->ParseSpecialValue(idName, parser);
						break;
					}
					//not special, just store it
					//fall thought

				case TOKEN_NUMBER:
				case TOKEN_STRING:
					this->SetString(idName, value);
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

	const String_c &Dictionary_c::GetString(const String_c &key) const
	{
		return Dictionary_c::GetValue(this, key).strValue;		
	}

	bool Dictionary_c::TryGetString(const String_c &key, String_c &value) const
	{
		const String_c *foundValue = TryGetString(this, key);
		if(!foundValue)
			return false;

		value = *foundValue;
		return true;
	}

	const String_c *Dictionary_c::TryGetString(const String_c &key) const
	{
		return TryGetString(this, key);
	}

	bool Dictionary_c::GetBool(const String_c &key) const
	{
		const String_c &value = this->GetString(key);

		return value.compare("true") == 0 ? true : false;
	}

	void Dictionary_c::Get4Float(float values[4], const String_c &key) const
	{
		const String_c &value = this->GetString(key);

		sscanf(value.c_str(), "%f %f %f %f", &values[0], &values[1], &values[2], &values[3]);
	}

	bool Dictionary_c::TryGet4Float(float values[4], const String_c &key) const
	{
		const String_c *value = this->TryGetString(key);
		if(!value)
			return false;

		sscanf(value->c_str(), "%f %f %f %f", &values[0], &values[1], &values[2], &values[3]);

		return true;
	}

	void Dictionary_c::Get3Float(float values[3], const String_c &key) const
	{
		const String_c &value = this->GetString(key);

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
		return StringToInt(this->GetString(key));
	}

	float Dictionary_c::GetFloat(const String_c &key) const
	{
		return StringToFloat(this->GetString(key));
	}

	bool Dictionary_c::TryGetFloat(float &outValue, const String_c &key) const
	{
		const String_c *strValue = this->TryGetString(key);
		if(!strValue)
			return false;

		outValue = StringToFloat(*strValue);

		return true;
	}

	const Dictionary_c::MatrixDataHandle_c Dictionary_c::GetMatrix(const String_c &key) const
	{
		const Value_s &value = GetValue(this, key);

		if(value.eType != CHAR_MATRIX)
		{
			std::stringstream stream;
			stream << "Value " << key << " is not a matrix " << this->GetName();
			PH_RAISE(INVALID_OPERATION_EXCEPTION, "Dictionary_c::GetMatrix", stream.str());
		}

		return MatrixDataHandle_c(value);
	}

	const Dictionary_c::Value_s *Dictionary_c::TryGetValue(const Dictionary_c *current, const String_c &key)
	{
		do
		{
			ValueMap_t::const_iterator it = current->mapValues.find(key);
			if(it != current->mapValues.end())
			{
				return &it->second;
			}

			current = current->GetInherited();
		} while(current);

		return NULL;
	}

	const Dictionary_c::Value_s &Dictionary_c::GetValue(const Dictionary_c *current, const String_c &key)
	{
		const Value_s *foundValue = TryGetValue(current, key);
		if(!foundValue)
		{
			std::stringstream stream;
			stream << "Value " << key << " does not exists in " << current->GetName();
			PH_RAISE(OBJECT_NOT_FOUND_EXCEPTION, "Dictionary_c::GetString", stream.str());
		}

		return *foundValue;
	}

	const String_c *Dictionary_c::TryGetString(const Dictionary_c *current, const String_c &key)
	{
		const Value_s *value = TryGetValue(current, key);

		return value != NULL ? &value->strValue : NULL;
	}	
}
