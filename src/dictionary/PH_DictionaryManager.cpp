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

#include "PH_DictionaryManager.h"

#include <fstream>

#include <boost/filesystem.hpp>

#include <PH_Context.h>
#include <PH_ContextUtils.h>
#include <PH_Error.h>
#include <PH_Exception.h>
#include <PH_Kernel.h>
#include <PH_Memory.h>
#include <PH_Parser.h>
#include <PH_Path.h>

#include "PH_Dictionary.h"
#include "PH_DictionaryHive.h"
#include "PH_DictionaryUtils.h"

namespace Phobos
{
	PH_DEFINE_NODE_SINGLETON(DictionaryManager, "/");

	DictionaryManager_c::DictionaryManager_c():
		Node_c("DictionaryManager", NodeFlags::PRIVATE_CHILDREN),
		cmdLoadAllDeclarations("loadAllDeclarations")
	{		
		cmdLoadAllDeclarations.SetProc(PH_CONTEXT_CMD_BIND(&DictionaryManager_c::CmdLoadAllDeclarations, this));
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

	DictionaryHive_c &DictionaryManager_c::CreateCustomHive(const String_c &name)
	{
		std::auto_ptr<DictionaryHive_c> hive(PH_NEW DictionaryHive_c(name));
		this->AddPrivateChild(*hive);

		hive->SetManaged(true);

		return *(hive.release());
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

			DictionaryHive_c *hive = this->TryGetDictionaryHive(tokenValue);
			if(!hive)
			{
				std::auto_ptr<DictionaryHive_c> hivePtr(PH_NEW DictionaryHive_c(tokenValue));
				
				//load before adding, so if error occurs it is not added
				hivePtr->Load(parser);

				this->AddPrivateChild(*hivePtr);

				hivePtr->SetManaged(true);
				hivePtr.release();
			}			
			else
				hive->Load(parser);
		}
	}

	void DictionaryManager_c::LoadAll(const String_c &path)
	{
		using namespace std;
		using namespace boost::filesystem;

		if(!exists(path))
		{
			stringstream stream;

			stream << "path " << path << " does not exists." << endl;
			PH_RAISE(INVALID_PARAMETER_EXCEPTION, "DictionaryManager_c::LoadAll", stream.str());
		}

		if(!is_directory(path))
		{
			stringstream stream;

			stream << "path " << path << " is not a directory." << endl;
			PH_RAISE(INVALID_PARAMETER_EXCEPTION, "DictionaryManager_c::LoadAll", stream.str());
		}

		directory_iterator end;
		for(directory_iterator it(path);it != end; ++it)
		{
			if(!is_regular_file(it->path()))
				continue;

			this->Load(it->path().string());
		}
	}

	DictionaryHive_c &DictionaryManager_c::GetDictionaryHive(const String_c &name)
	{
		return static_cast<DictionaryHive_c&>(this->GetChild(name));
	}

	DictionaryHive_c *DictionaryManager_c::TryGetDictionaryHive(const String_c &name)
	{
		return static_cast<DictionaryHive_c*>(this->TryGetChild(name));
	}

	Dictionary_c &DictionaryManager_c::GetDictionary(const String_c &hive, const String_c &dictionary)
	{
		return this->GetDictionaryHive(hive).GetDictionary(dictionary);
	}

	Dictionary_c *DictionaryManager_c::TryGetDictionary(const String_c &hive, const String_c &dictionary)
	{
		DictionaryHive_c *hivePtr = this->TryGetDictionaryHive(hive);
		
		return hivePtr ? hivePtr->TryGetDictionary(dictionary) : NULL;
	}

	Dictionary_c &DictionaryManager_c::GetDictionary(const Path_c &relativePath)
	{
		Path_c tmp;
		this->GetThisPath(tmp);

		tmp.Add(relativePath);

		Node_c &node = Kernel_c::GetInstance().LookupObject(tmp);

		return static_cast<Dictionary_c &>(node);
	}

	void DictionaryManager_c::RegisterCommands(IContext_c &context)
	{
		context.AddContextCmd(cmdLoadAllDeclarations);
	}

	void DictionaryManager_c::CmdLoadAllDeclarations(const StringVector_t &args, Context_c &context)
	{
		if(args.size() < 2)
		{
			Kernel_c::GetInstance().LogMessage("[DictionaryManager_c::CmdLoadAllDeclarations] Insuficient parameters, usage: loadAllDeclarations <path>\n");

			return;
		}

		for(int i = 1, len = args.size(); i < len; ++i)
			this->LoadAll(args[i]);
	}
}

