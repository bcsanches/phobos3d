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

#include "Phobos/Register/Manager.h"

#include <fstream>
#include <memory>

#include <boost/filesystem.hpp>

#include <PH_Context.h>
#include <PH_ContextUtils.h>
#include <PH_Error.h>
#include <PH_Exception.h>
#include <PH_Kernel.h>
#include <PH_Node.h>
#include <PH_Memory.h>
#include <PH_Parser.h>
#include <PH_Path.h>

#include "Phobos/Register/Hive.h"
#include "Phobos/Register/Table.h"
#include "Phobos/Register/Utils.h"

namespace Phobos
{
	namespace Register
	{
		namespace
		{
			class Manager_c: public Node_c
			{
				public:
					Manager_c();

					void AddLocalPrivateChild(std::unique_ptr<Node_c> &&ptr);
			};

			Manager_c::Manager_c():
				Node_c("Register", NodeFlags::PRIVATE_CHILDREN)
			{
				//empty
			}

			inline void Manager_c::AddLocalPrivateChild(std::unique_ptr<Node_c> &&ptr)
			{
				this->AddPrivateChild(std::move(ptr));
			}

			void CmdLoadAllDeclarations(const StringVector_t &args, Context_c &)
			{	
				if(args.size() < 2)
				{
					Kernel_c::GetInstance().LogMessage("[DictionaryManager_c::CmdLoadAllDeclarations] Insuficient parameters, usage: loadAllDeclarations <path>\n");

					return;
				}

				for(int i = 1, len = args.size(); i < len; ++i)
					LoadAll(args[i]);	
			}

			static ContextCmd_c cmdLoadAllDeclarations_gl("loadAllDeclarations", CmdLoadAllDeclarations);
			static std::unique_ptr<Manager_c> spManager_gl;
		}
	}
}

void Phobos::Register::Init()
{
	spManager_gl.reset(PH_NEW(Manager_c));

	Kernel_c::GetInstance().AddObject(*spManager_gl, Path_c("/"));
}

void Phobos::Register::Finalize()
{
	spManager_gl.reset();
}

void Phobos::Register::Load(const String_c &fileName)
{
	using namespace std;

	ifstream file(fileName.c_str());

	Load(file);
}

Phobos::Register::Hive_c &Phobos::Register::CreateCustomHive(const String_c &name)
{
	std::unique_ptr<Hive_c> hivePtr(PH_NEW Hive_c(name));

	auto hive = hivePtr.get();

	spManager_gl->AddLocalPrivateChild(std::move(hivePtr));

	return *hive;
}

void Phobos::Register::Load(std::istream &file)
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
			RaiseParseException(parser, TOKEN_ID, token, tokenValue, "DictionaryManager_c::Load");

		Hive_c *hive = TryGetHive(tokenValue);
		if(!hive)
		{
			std::unique_ptr<Hive_c> hivePtr(PH_NEW Hive_c(tokenValue));
				
			//load before adding, so if error occurs it is not added
			hivePtr->Load(parser);

			spManager_gl->AddLocalPrivateChild(std::move(hivePtr));				
		}			
		else
			hive->Load(parser);
	}
}

void Phobos::Register::LoadAll(const String_c &path)
{
	using namespace std;
	using namespace boost::filesystem;

	if(!exists(path))
	{
		stringstream stream;

		stream << "path " << path << " does not exists." << endl;
		PH_RAISE(INVALID_PARAMETER_EXCEPTION, "Register::LoadAll", stream.str());
	}

	if(!is_directory(path))
	{
		stringstream stream;

		stream << "path " << path << " is not a directory." << endl;
		PH_RAISE(INVALID_PARAMETER_EXCEPTION, "Register::LoadAll", stream.str());
	}

	directory_iterator end;
	for(directory_iterator it(path);it != end; ++it)
	{
		if(!is_regular_file(it->path()))
			continue;

		Load(it->path().string());
	}
}

Phobos::Register::Hive_c &Phobos::Register::GetHive(const String_c &name)
{
	return static_cast<Hive_c&>(spManager_gl->GetChild(name));
}

Phobos::Register::Hive_c *Phobos::Register::TryGetHive(const String_c &name)
{
	return static_cast<Hive_c*>(spManager_gl->TryGetChild(name));
}

Phobos::Register::Table_c &Phobos::Register::GetTable(const String_c &hive, const String_c &table)
{
	return GetHive(hive).GetTable(table);
}

Phobos::Register::Table_c *Phobos::Register::TryGetTable(const String_c &hive, const String_c &table)
{
	Hive_c *hivePtr = TryGetHive(hive);
		
	return hivePtr ? hivePtr->TryGetTable(table) : NULL;
}

Phobos::Register::Table_c &Phobos::Register::GetTable(const Path_c &relativePath)
{
	Path_c tmp;
	spManager_gl->GetThisPath(tmp);

	tmp.Add(relativePath);

	Node_c &node = Kernel_c::GetInstance().LookupObject(tmp);

	return static_cast<Table_c &>(node);
}

void Phobos::Register::RegisterCommands(IContext_c &context)
{
	context.AddContextCmd(cmdLoadAllDeclarations_gl);
}



