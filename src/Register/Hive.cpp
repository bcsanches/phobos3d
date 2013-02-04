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

#include "Phobos/Register/Hive.h"

#include <Phobos/Memory.h>
#include <Phobos/Parser.h>

#include "Phobos/Register/Manager.h"
#include "Phobos/Register/Table.h"
#include "Phobos/Register/Utils.h"

Phobos::Register::HivePtr_t Phobos::Register::Hive::Create(const String_t &name)
{
	return HivePtr_t(PH_NEW Hive(name));
}

Phobos::Register::Hive::Hive(const String_t &name):
	Node(name, NodeFlags::PRIVATE_CHILDREN),
	m_uSequence(0)
{
	//empty
}

Phobos::Register::Hive::~Hive()
{
	//empty
}

void Phobos::Register::Hive::AddTable(std::unique_ptr<Table> &&dict)
{				
	this->AddPrivateChild(std::move(dict));
}

void Phobos::Register::Hive::Load(Parser &parser)
{
	String_t tokenValue;
	ParserTokens_e token = parser.GetToken(&tokenValue);

	String_t dictName;
	String_t inherit;
	String_t baseHive;

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
			RaiseParseException(parser, TOKEN_ID, token, tokenValue, "DictionaryHive::Load");
		}			

		token = parser.GetToken(&tokenValue);
		if(token == TOKEN_DOT)
		{
			token = parser.GetToken(&inherit);
			if(token != TOKEN_ID)
			{
				RaiseParseException(parser, TOKEN_ID, token, tokenValue, "DictionaryHive::Load");
			}				

			//grab next token (must be a {, to be handled later if nameless)
			token = parser.GetToken(&tokenValue);
		}
		else if(token != TOKEN_OPEN_BRACE)
			RaiseParseException(parser, TOKEN_OPEN_BRACE, token, tokenValue, "DictionaryHive::Load");
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
			dictName += std::to_string(m_uSequence++);
		}

		parser.PushToken();
	}		

	std::unique_ptr<Table> dict(PH_NEW Table(dictName));		

	if(!baseHive.empty())
		dict->SetBaseHive(baseHive);

	if(!inherit.empty())
		dict->SetInherited(inherit);

	dict->Load(parser);		

	this->AddTable(std::move(dict));
}

Phobos::Register::Table &Phobos::Register::Hive::GetTable(const String_t &name)
{
	return static_cast<Table &>(this->GetChild(name));
}

Phobos::Register::Table *Phobos::Register::Hive::TryGetTable(const String_t &name)
{
	return static_cast<Phobos::Register::Table *>(this->TryGetChild(name));
}