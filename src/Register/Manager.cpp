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

#include <Phobos/Shell/Context.h>
#include <Phobos/Shell/Utils.h>

#include <Phobos/Error.h>
#include <Phobos/Exception.h>
#include <Phobos/Log.h>
#include <Phobos/Node.h>
#include <Phobos/Memory.h>
#include <Phobos/ObjectManager.h>
#include <Phobos/Parser.h>
#include <Phobos/Path.h>

#include "Phobos/Register/Hive.h"
#include "Phobos/Register/Table.h"
#include "Phobos/Register/Utils.h"

namespace Phobos
{
	namespace Register
	{
		namespace
		{
			class Manager: public Node
			{
				public:
					Manager();

					void AddLocalPrivateChild(std::unique_ptr<Node> &&ptr);
			};

			Manager::Manager():
				Node("Register", NodeFlags::PRIVATE_CHILDREN)
			{
				ObjectManager::AddObject(*this, Path("/"));
			}

			inline void Manager::AddLocalPrivateChild(std::unique_ptr<Node> &&ptr)
			{
				this->AddPrivateChild(std::move(ptr));
			}

			void CmdLoadAllDeclarations(const Shell::StringVector_t &args, Shell::Context &)
			{	
				if(args.size() < 2)
				{
					LogMessage("[DictionaryManager::CmdLoadAllDeclarations] Insuficient parameters, usage: loadAllDeclarations <path>\n");

					return;
				}

				for(int i = 1, len = args.size(); i < len; ++i)
					LoadAll(args[i]);	
			}

			static Shell::Command cmdLoadAllDeclarations_gl("loadAllDeclarations", CmdLoadAllDeclarations);
			static std::unique_ptr<Manager> spManager_gl(PH_NEW(Manager));
		}
	}
}

void Phobos::Register::Clear()
{
	spManager_gl->RemoveAllChildren();
}

void Phobos::Register::Load(StringRef_t fileName)
{
	using namespace std;

	ifstream file(fileName.data());

	Load(file);
}

Phobos::Register::Hive &Phobos::Register::CreateCustomHive(const String_t &name)
{
	std::unique_ptr<Hive> hivePtr(PH_NEW Hive(name));

	auto hive = hivePtr.get();

	spManager_gl->AddLocalPrivateChild(std::move(hivePtr));

	return *hive;
}

void Phobos::Register::Load(std::istream &file)
{
	Parser parser;
	parser.SetStream(&file);

	String_t tokenValue;
	for(;;)
	{
		ParserTokens_e token = parser.GetToken(&tokenValue);

		if(token == TOKEN_EOF)
			break;

		if(token != TOKEN_ID)
			RaiseParseException(parser, TOKEN_ID, token, tokenValue, "DictionaryManager::Load");

		Hive *hive = TryGetHive(tokenValue);
		if(!hive)
		{
			std::unique_ptr<Hive> hivePtr(PH_NEW Hive(tokenValue));
				
			//load before adding, so if error occurs it is not added
			hivePtr->Load(parser);

			spManager_gl->AddLocalPrivateChild(std::move(hivePtr));				
		}			
		else
			hive->Load(parser);
	}
}

void Phobos::Register::LoadAll(const String_t &path)
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

Phobos::Register::Hive &Phobos::Register::GetOrCreateHive(const String_t &name)
{
	auto *hive = TryGetHive(name);

	return (hive == nullptr) ? CreateCustomHive(name) : *hive;	
}

Phobos::Register::Hive &Phobos::Register::GetHive(const String_t &name)
{
	return static_cast<Hive&>(spManager_gl->GetChild(name));
}

Phobos::Register::Hive *Phobos::Register::TryGetHive(const String_t &name)
{
	return static_cast<Hive*>(spManager_gl->TryGetChild(name));
}

Phobos::Register::Table &Phobos::Register::GetTable(const String_t &hive, const String_t &table)
{
	return GetHive(hive).GetTable(table);
}

Phobos::Register::Table *Phobos::Register::TryGetTable(const String_t &hive, const String_t &table)
{
	Hive *hivePtr = TryGetHive(hive);
		
	return hivePtr ? hivePtr->TryGetTable(table) : NULL;
}

Phobos::Register::Table &Phobos::Register::GetTable(const Path &relativePath)
{
	Path tmp;
	spManager_gl->GetThisPath(tmp);

	tmp.Add(relativePath);

	Node &node = ObjectManager::LookupObject(tmp);

	return static_cast<Table &>(node);
}

void Phobos::Register::RegisterCommands(Shell::IContext &context)
{
	context.AddContextCommand(cmdLoadAllDeclarations_gl);
}



