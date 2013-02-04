/*
Phobos 3d
September 2010
Copyright (c) 2005-2013 Bruno Sanches  http://code.google.com/p/phobos3d

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#include "Phobos/Register/Table.h"

#include <PH_Exception.h>
#include <PH_Memory.h>
#include <PH_Parser.h>

#include "Phobos/Register/Manager.h"
#include "Phobos/Register/Utils.h"
#include <stdio.h>

#define INHERIT_KEY "inherit"
#define BASE_HIVE_KEY "baseHive"

#define NEW_KEYWORD "new"

namespace
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
}

Phobos::Register::TablePtr_t Phobos::Register::Table_c::Create(const String_t &name)
{
	return TablePtr_t(PH_NEW Table_c(name));
}

Phobos::Register::Table_c::Table_c(const String_t &name):
	Node_c(name),
	pclInherit(NULL)
{
	//empty
}

Phobos::Register::Table_c::~Table_c()
{
	//empty
}

void Phobos::Register::Table_c::CheckInvalidKey(const String_t &key, const char *keys[], const char *message) const
{
	for(int i = 0;keys[i]; ++i)
	{
		if(key.compare(keys[i]) == 0)
		{
			std::stringstream stream;
			stream << "Value " << key << " " << message;
			PH_RAISE(INVALID_PARAMETER_EXCEPTION, "Phobos::Register::Table_c::CheckInvalidKey", stream.str());
		}
	}
}

void Phobos::Register::Table_c::CheckForKeyword(const String_t &key) const
{
	this->CheckInvalidKey(key, parszKeywords_g, "cannot be a key because it is a reserved keyword");		
}

void Phobos::Register::Table_c::SetInherited(const String_t &base)
{
	this->SetString(INHERIT_KEY, base);
}

void Phobos::Register::Table_c::SetBaseHive(const String_t &baseHive)
{
	this->SetString(BASE_HIVE_KEY, baseHive);
}

void Phobos::Register::Table_c::SetString(const String_t &key, const String_t &value)
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

void Phobos::Register::Table_c::SetCharMatrix(const String_t &key, const String_t &data, UInt16_t numRows, UInt16_t numColumns)
{	
	CheckForKeyword(key);
	this->CheckInvalidKey(key, parszStringOnlyKeys_g, "should be string data, not matrix");

	if(numRows * numColumns == 0)
	{
		PH_RAISE(INVALID_PARAMETER_EXCEPTION, "Phobos::Register::Table_c::ParseSpecialValue", "Matrix cannot be empty");
	}

	if(numRows * numColumns != data.length())
	{
		std::stringstream stream;
		stream << "Matrix data size (" << data.length() << ") does not match width (" << numColumns << ") and height (" << numRows << ") parameters";
		PH_RAISE(INVALID_PARAMETER_EXCEPTION, "Phobos::Register::Table_c::ParseSpecialValue", stream.str());
	}		

	mapValues[key] = Value_s(data, numRows, numColumns);
}

void Phobos::Register::Table_c::ParseSpecialValue(const String_t &idName, Parser_c &parser)
{
	String_t type;

	ParserTokens_e token;
	if((token = parser.GetToken(&type)) != TOKEN_ID)
	{
		RaiseParseException(parser, TOKEN_ID, token, type, "Phobos::Register::Table_c::ParseSpecialValue");
	}

	if(type.compare("CharMatrix") == 0)
	{
		if((token = parser.GetToken(NULL)) != TOKEN_OPEN_PAREN)
		{
			RaiseParseException(parser, TOKEN_OPEN_PAREN, token, type, "Phobos::Register::Table_c::ParseSpecialValue");
		}

		String_t matrix;
		String_t row;

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
					RaiseParseException(parser, "matrix data", "closing parenthesis", "Phobos::Register::Table_c::ParseSpecialValue");
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
						PH_RAISE(PARSER_EXCEPTION, "Phobos::Register::Table_c::ParseSpecialValue", "Matrix cannot be empty");

					first = false;
				}
				else if(numColumns != row.length())
				{
					PH_RAISE(PARSER_EXCEPTION, "Phobos::Register::Table_c::ParseSpecialValue", "Matrix rows should always have the same length");
				}

				matrix.append(row);
				++numRows;
			}
			else
			{
				RaiseParseException(parser, TOKEN_STRING, token, row, "Phobos::Register::Table_c::ParseSpecialValue");
			}
		}
	}	
	else
	{
		RaiseParseException(parser, " valid especial type, ie CharMatrix", type.c_str(), "Phobos::Register::Table_c::ParseSpecialValue");
	}
}

void Phobos::Register::Table_c::Load(Parser_c &parser)
{
	String_t idName;
	String_t value;

	ParserTokens_e token = parser.GetToken(&value);

	if(token != TOKEN_OPEN_BRACE)
		RaiseParseException(parser, TOKEN_OPEN_BRACE, token, value, "Phobos::Register::Table_c::Load");

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
				RaiseParseException(parser, TOKEN_ID, token, value, "Phobos::Register::Table_c::Load");

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

			RaiseParseException(parser, TOKEN_EQUAL, token, value, "Phobos::Register::Table_c::Load");
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
				RaiseParseException(parser, TOKEN_STRING, token, idName, "Phobos::Register::Table_c::Load");
				break;
		}

		token = parser.GetToken(&value);
		if(token != TOKEN_SEMI_COLON)
			RaiseParseException(parser, TOKEN_SEMI_COLON, token, value, "Phobos::Register::Table_c::Load");
	}
}

const Phobos::String_t &Phobos::Register::Table_c::GetString(const String_t &key) const
{
	return Phobos::Register::Table_c::GetValue(this, key).strValue;		
}

bool Phobos::Register::Table_c::TryGetString(const String_t &key, String_t &value) const
{
	const String_t *foundValue = TryGetString(this, key);
	if(!foundValue)
		return false;

	value = *foundValue;
	return true;
}

const Phobos::String_t *Phobos::Register::Table_c::TryGetString(const String_t &key) const
{
	return TryGetString(this, key);
}

bool Phobos::Register::Table_c::GetBool(const String_t &key) const
{
	const String_t &value = this->GetString(key);

	return value.compare("true") == 0 ? true : false;
}
	
bool Phobos::Register::Table_c::TryGetBool(bool &outValue, const String_t &key) const
{
	const String_t *value = this->TryGetString(key);
	if(!value)
		return false;

	outValue = value->compare("true") == 0;

	return true;
}

void Phobos::Register::Table_c::Get4Float(float values[4], const String_t &key) const
{
	const String_t &value = this->GetString(key);

	sscanf(value.c_str(), "%f %f %f %f", &values[0], &values[1], &values[2], &values[3]);
}

bool Phobos::Register::Table_c::TryGet4Float(float values[4], const String_t &key) const
{
	const String_t *value = this->TryGetString(key);
	if(!value)
		return false;

	sscanf(value->c_str(), "%f %f %f %f", &values[0], &values[1], &values[2], &values[3]);

	return true;
}

void Phobos::Register::Table_c::Get3Float(float values[3], const String_t &key) const
{
	const String_t &value = this->GetString(key);

	sscanf(value.c_str(), "%f %f %f", &values[0], &values[1], &values[2]);
}

const Phobos::Register::Table_c *Phobos::Register::Table_c::GetInherited() const
{
	if(pclInherit)
		return pclInherit;

	if(!strInherit.empty())
	{
		if(strBaseHive.empty())
			pclInherit = &static_cast<Phobos::Register::Table_c &>(this->GetParent()->GetChild(strInherit));
		else
			pclInherit = &Phobos::Register::GetTable(strBaseHive, strInherit);
	}

	return pclInherit;
}

int Phobos::Register::Table_c::GetInt(const String_t &key) const
{
	return StringToInt(this->GetString(key));
}

float Phobos::Register::Table_c::GetFloat(const String_t &key) const
{
	return StringToFloat(this->GetString(key));
}

bool Phobos::Register::Table_c::TryGetFloat(float &outValue, const String_t &key) const
{
	const String_t *strValue = this->TryGetString(key);
	if(!strValue)
		return false;

	outValue = StringToFloat(*strValue);

	return true;
}

const Phobos::Register::Table_c::MatrixDataHandle_c Phobos::Register::Table_c::GetMatrix(const String_t &key) const
{
	const Value_s &value = GetValue(this, key);

	if(value.eType != CHAR_MATRIX)
	{
		std::stringstream stream;
		stream << "Value " << key << " is not a matrix " << this->GetName();
		PH_RAISE(INVALID_OPERATION_EXCEPTION, "Phobos::Register::Table_c::GetMatrix", stream.str());
	}

	return MatrixDataHandle_c(value);
}

const Phobos::Register::Table_c::Value_s *Phobos::Register::Table_c::TryGetValue(const Table_c *current, const String_t &key)
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

const Phobos::Register::Table_c::Value_s &Phobos::Register::Table_c::GetValue(const Table_c *current, const String_t &key)
{
	const Value_s *foundValue = TryGetValue(current, key);
	if(!foundValue)
	{
		std::stringstream stream;
		stream << "Value " << key << " does not exists in " << current->GetName();
		PH_RAISE(OBJECT_NOT_FOUND_EXCEPTION, "Phobos::Register::Table_c::GetString", stream.str());
	}

	return *foundValue;
}

const Phobos::String_t *Phobos::Register::Table_c::TryGetString(const Table_c *current, const String_t &key)
{
	const Value_s *value = TryGetValue(current, key);

	return value != NULL ? &value->strValue : NULL;
}	
