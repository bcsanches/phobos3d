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

#include "PH_DictionaryManager.h"

#include <fstream>

#include <PH_Error.h>
#include <PH_Parser.h>

#include "PH_Dictionary.h"
#include "PH_DictionaryHive.h"
#include "PH_DictionaryUtils.h"

namespace Phobos
{
	PH_DEFINE_DEFAULT_SINGLETON(DictionaryManager);

	DictionaryManager_c::DictionaryManager_c():
		Node_c("DictionaryManager", PRIVATE_CHILDREN)
	{
		//empty
	}

	DictionaryManager_c::~DictionaryManager_c()
	{
	}

	void DictionaryManager_c::Load(const String_c &fileName)
	{
		using namespace std;

		ifstream file(fileName.c_str());

		this->Load(file);
	}

	void DictionaryManager_c::Load(std::istream &file)
	{
		Parser_c parser;
		parser.SetStream(&file);

		String_c tokenValue;
		for(;;)
		{
			ParserTokens_e token = parser.GetToken(&tokenValue);

			if(token == TOKEN_EOF)
				break;

			if(token != TOKEN_ID)
				PH_RaiseDictionaryParseException(parser, TOKEN_ID, token, tokenValue, "DictionaryManager_c::Load");

			DictionaryHivePtr_t hive = boost::static_pointer_cast<DictionaryHive_c>(this->TryGetChild(tokenValue));
			if(!hive)
			{
				hive = DictionaryHive_c::Create(tokenValue);
				
				//load before adding, so if error occurs it is not added
				hive->Load(parser);

				this->AddPrivateChild(hive);
			}			
			else
				hive->Load(parser);
		}
	}

	DictionaryHivePtr_t DictionaryManager_c::GetDictionaryHive(const String_c &name)
	{
		return boost::static_pointer_cast<DictionaryHive_c>(this->GetChild(name));
	}

	DictionaryPtr_t DictionaryManager_c::GetDictionary(const String_c &hive, const String_c &dictionary)
	{
		return this->GetDictionaryHive(hive)->GetDictionary(dictionary);
	}
}

