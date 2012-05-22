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


#include <boost/test/unit_test.hpp>

#include <PH_Dictionary.h>
#include <PH_DictionaryHive.h>
#include <PH_DictionaryManager.h>
#include <PH_Exception.h>
#include <PH_Kernel.h>

using namespace Phobos;
using namespace std;

struct KernelInstance_s
{
	KernelInstance_s()
	{
		Kernel_c::CreateInstance("dictionarytest.log");	
		DictionaryManager_c::CreateInstance();
	}

	~KernelInstance_s()
	{		
		DictionaryManager_c::ReleaseInstance();

		Kernel_c::ReleaseInstance();
	}
};

BOOST_AUTO_TEST_CASE(dictionary_basic)
{
	KernelInstance_s instance;

	stringstream stream;

	stream << "EntityDef InfoPlayerStart" << endl;
	stream << "{" << endl;
	stream <<	"className=Entity;" << endl;
	stream <<	"health=100;" << endl;
	stream <<	"description=\"bla bla\";" << endl;
	stream <<   "Composed.Property=\"test\";" << endl;
	stream <<   "Composed.Property.test2=\"test2\";" << endl;
	stream << "}" << endl;
	stream << endl << endl;
	stream << "RigidBody PlayerCapsule" << endl;
	stream << "{" << endl;
	stream <<	"height=1.0;" << endl;
	stream <<	"radius=2.0;" << endl;
	stream << "}";

	DictionaryManagerPtr_t manager = DictionaryManager_c::GetInstance();

	manager->Load(stream);
}

BOOST_AUTO_TEST_CASE(dictionary_errors)
{
	KernelInstance_s instance;

	DictionaryManagerPtr_t manager = DictionaryManager_c::GetInstance();

	{
		//invalid syntax
		stringstream stream;
		
		stream << "{" << endl;

		BOOST_REQUIRE_THROW(manager->Load(stream), ParserException_c);
	}

	{
		//invalid syntax
		stringstream stream;
		
		//missing hive
		stream << "EntityDef " << endl;
		stream << "{" << endl;

		BOOST_REQUIRE_THROW(manager->Load(stream), ParserException_c);
	}

	{
		//invalid syntax
		stringstream stream;
		
		stream << "EntityDef InfoPlayerStart" << endl;
		stream << "{" << endl;
		//missing =
		stream <<	"className Entity" << endl;

		BOOST_REQUIRE_THROW(manager->Load(stream), ParserException_c);
	}

	{
		//invalid syntax
		stringstream stream;
		
		stream << "EntityDef InfoPlayerStart" << endl;
		stream << "{" << endl;
		//missing ;
		stream <<	"className=Entity" << endl;
		stream <<	"health=100;" << endl;

		BOOST_REQUIRE_THROW(manager->Load(stream), ParserException_c);
	}

	{
		stringstream stream;

		//duplicated entry
		stream << "EntityDef InfoPlayerStart" << endl;
		stream << "{" << endl;
		stream <<	"className=Entity;" << endl;
		stream <<	"health=100;" << endl;
		stream <<	"description=\"bla bla\";" << endl;
		stream << "}" << endl;
		stream << endl << endl;
		stream << "EntityDef InfoPlayerStart" << endl;
		stream << "{" << endl;
		stream <<	"height=1.0;" << endl;
		stream <<	"radius=2.0;" << endl;
		stream << "}";

		BOOST_REQUIRE_THROW(manager->Load(stream), ObjectAlreadyExistsException_c);
	}
}

BOOST_AUTO_TEST_CASE(dictionary_inheritance)
{
	KernelInstance_s instance;

	stringstream stream;

	stream << "EntityDef InfoPlayerStart" << endl;
	stream << "{" << endl;
	stream <<	"className=Entity;" << endl;
	stream <<	"health=100;" << endl;
	stream <<	"weight=2.0;" << endl;
	stream <<	"description=\"bla bla\";" << endl;
	stream << "}" << endl;
	stream << endl << endl;
	stream << "EntityDef SuperPlayer" << endl;
	stream << "{" << endl;
	stream <<	"inherit=InfoPlayerStart;" << endl;
	stream <<	"health=200;" << endl;	
	stream <<	"boost=2;" << endl;	
	stream << "}";
	stream << "EntityDef InvalidPlayer" << endl;
	stream << "{" << endl;
	stream <<	"inherit=NonExistingDef;" << endl;
	stream <<	"health=200;" << endl;	
	stream << "}";

	DictionaryManagerPtr_t manager = DictionaryManager_c::GetInstance();

	manager->Load(stream);

	//test both forms of retrieving data
	DictionaryPtr_t infoPlayerStart = manager->GetDictionary("EntityDef", "InfoPlayerStart");
	DictionaryPtr_t superPlayer = manager->GetDictionaryHive("EntityDef")->GetDictionary("SuperPlayer");

	//now check overriding
	BOOST_REQUIRE(infoPlayerStart->GetString("health").compare("100") == 0);
	BOOST_REQUIRE(superPlayer->GetString("health").compare("200") == 0);

	//check parent relationship
	BOOST_REQUIRE(infoPlayerStart->GetString("weight").compare("2.0") == 0);
	BOOST_REQUIRE(superPlayer->GetString("weight").compare("2.0") == 0);

	//non existing value
	BOOST_REQUIRE_THROW(infoPlayerStart->GetString("boost"), ObjectNotFoundException_c);
	BOOST_REQUIRE(superPlayer->GetString("boost").compare("2") == 0);

	//bad inheritance
	DictionaryPtr_t invalidPlayer = manager->GetDictionary("EntityDef", "InvalidPlayer");

	BOOST_REQUIRE_THROW(infoPlayerStart->GetString("bla"), ObjectNotFoundException_c);

	//Check getters exceptions
	BOOST_REQUIRE_THROW(manager->GetDictionaryHive("Bla"), ObjectNotFoundException_c);
	BOOST_REQUIRE_THROW(manager->GetDictionary("Bla", "Bla"), ObjectNotFoundException_c);
	BOOST_REQUIRE_THROW(manager->GetDictionary("EntityDef", "Bla"), ObjectNotFoundException_c);

	BOOST_REQUIRE_THROW(infoPlayerStart->SetString("new", "invalid keyword"), InvalidParameterException_c);
}

/**
	The same as dictionary_inheritance, but the inheritance chain uses a baseHive

*/
BOOST_AUTO_TEST_CASE(dictionary_inheritance_other_hive)
{
	KernelInstance_s instance;

	stringstream stream;

	stream << "EntityDef InfoPlayerStart" << endl;
	stream << "{" << endl;
	stream <<	"className=Entity;" << endl;
	stream <<	"health=100;" << endl;
	stream <<	"weight=2.0;" << endl;
	stream <<	"description=\"bla bla\";" << endl;
	stream << "}" << endl;
	stream << endl << endl;
	stream << "DynamicEntity SuperPlayer" << endl;
	stream << "{" << endl;
	stream <<	"inherit=InfoPlayerStart;" << endl;
	stream <<	"baseHive=EntityDef;" << endl;
	stream <<	"health=200;" << endl;	
	stream <<	"boost=2;" << endl;	
	stream << "}";
	stream << "DynamicEntity InvalidPlayer" << endl;
	stream << "{" << endl;
	stream <<	"inherit=InfoPlayerStart;" << endl;
	stream <<	"health=200;" << endl;	
	stream << "}";

	DictionaryManagerPtr_t manager = DictionaryManager_c::GetInstance();

	manager->Load(stream);

	//test both forms of retrieving data
	DictionaryPtr_t infoPlayerStart = manager->GetDictionary("EntityDef", "InfoPlayerStart");
	DictionaryPtr_t superPlayer = manager->GetDictionaryHive("DynamicEntity")->GetDictionary("SuperPlayer");

	//now check overriding
	BOOST_REQUIRE(infoPlayerStart->GetString("health").compare("100") == 0);
	BOOST_REQUIRE(superPlayer->GetString("health").compare("200") == 0);

	//check parent relationship
	BOOST_REQUIRE(infoPlayerStart->GetString("weight").compare("2.0") == 0);
	BOOST_REQUIRE(superPlayer->GetString("weight").compare("2.0") == 0);

	//non existing value
	BOOST_REQUIRE_THROW(infoPlayerStart->GetString("boost"), ObjectNotFoundException_c);
	BOOST_REQUIRE(superPlayer->GetString("boost").compare("2") == 0);

	//bad inheritance
	DictionaryPtr_t invalidPlayer = manager->GetDictionary("DynamicEntity", "InvalidPlayer");

	BOOST_REQUIRE_THROW(infoPlayerStart->GetString("bla"), ObjectNotFoundException_c);	
}

BOOST_AUTO_TEST_CASE(dictionary_parse_matrix)
{
	KernelInstance_s instance;

	stringstream stream;

	stream << "StaticEntities World" << endl;
	stream << "{" << endl;
	stream <<	"map = new CharMatrix(" << endl;
	stream <<	"\"########\"," << endl;
	stream <<	"\"#......#\"," << endl;
    stream <<	"\"######.#\"," << endl;
    stream <<	"\"#......#\"," << endl;
    stream <<	"\"#.#.#.##\"," << endl;
    stream <<	"\"#.######\"," << endl;
    stream <<	"\"#......#\"," << endl;
    stream <<	"\"########\"," << endl;
	stream <<	"\"########\"," << endl; //exta line just to avoid width and height to be the same
	stream <<	");" << endl;
	stream << "}";

	DictionaryManagerPtr_t manager = DictionaryManager_c::GetInstance();

	manager->Load(stream);

	DictionaryPtr_t world = manager->GetDictionary("StaticEntities", "World");

	Dictionary_c::MatrixDataHandle_c handle = world->GetMatrix("map");

	BOOST_REQUIRE(handle(1, 1) == '.');
	BOOST_REQUIRE(handle(4, 3) == '.');

	BOOST_REQUIRE(handle.GetNumColumns() == 8);
	BOOST_REQUIRE(handle.GetNumRows() == 9);
}

BOOST_AUTO_TEST_CASE(dictionary_parse_matrix_errors)
{
	KernelInstance_s instance;

	DictionaryManagerPtr_t manager = DictionaryManager_c::GetInstance();

	{
		//Invalid special type
		stringstream stream;

		stream << "StaticEntities World" << endl;
		stream << "{" << endl;
		stream <<	"map = new Bla;" << endl;
		stream << "}";

		BOOST_REQUIRE_THROW(manager->Load(stream), ParserException_c);
	}


	{
		//Invalid token type after new
		stringstream stream;

		stream << "StaticEntities World" << endl;
		stream << "{" << endl;
		stream <<	"map = new 123;" << endl;
		stream << "}";		

		BOOST_REQUIRE_THROW(manager->Load(stream), ParserException_c);
	}		

	{
		//Expected ( after CharMatrix
		stringstream stream;

		stream << "StaticEntities World" << endl;
		stream << "{" << endl;
		stream <<	"map = new CharMatrix;" << endl;
		stream << "}";		

		BOOST_REQUIRE_THROW(manager->Load(stream), ParserException_c);
	}

	{
		//Expected () data for CharMatrix
		stringstream stream;

		stream << "StaticEntities World" << endl;
		stream << "{" << endl;
		stream <<	"map = new CharMatrix();" << endl;
		stream << "}";		

		BOOST_REQUIRE_THROW(manager->Load(stream), ParserException_c);
	}

	{
		//CharMatrix data cannot be empty
		stringstream stream;

		stream << "StaticEntities World" << endl;
		stream << "{" << endl;
		stream <<	"map = new CharMatrix(\"\");" << endl;
		stream << "}";		

		BOOST_REQUIRE_THROW(manager->Load(stream), ParserException_c);
	}

	{
		//CharMatrix should be simetric
		stringstream stream;

		stream << "StaticEntities World" << endl;
		stream << "{" << endl;
		stream <<	"map = new CharMatrix(\"123\",\"12\");" << endl;
		stream << "}";		

		BOOST_REQUIRE_THROW(manager->Load(stream), ParserException_c);
	}

	{
		//CharMatrix should be formed by strings
		stringstream stream;

		stream << "StaticEntities World" << endl;
		stream << "{" << endl;
		stream <<	"map = new CharMatrix(1223);" << endl;
		stream << "}";		

		BOOST_REQUIRE_THROW(manager->Load(stream), ParserException_c);
	}
}

BOOST_AUTO_TEST_CASE(dictionary_add_matrix_errors)
{
	KernelInstance_s instance;

	stringstream stream;

	stream << "EntityDef InfoPlayerStart" << endl;
	stream << "{" << endl;
	stream <<	"className=Entity;" << endl;
	stream <<	"health=100;" << endl;
	stream <<	"weight=2.0;" << endl;
	stream <<	"description=\"bla bla\";" << endl;
	stream << "}" << endl;

	DictionaryManagerPtr_t manager = DictionaryManager_c::GetInstance();

	manager->Load(stream);

	DictionaryPtr_t infoPlayerStart = manager->GetDictionary("EntityDef", "InfoPlayerStart");

	//Cannot use inherit as matrix
	BOOST_REQUIRE_THROW(infoPlayerStart->SetCharMatrix("inherit", "AB", 1, 1), InvalidParameterException_c);
	BOOST_REQUIRE_THROW(infoPlayerStart->SetCharMatrix("base_hive", "AB", 1, 1), InvalidParameterException_c);
	BOOST_REQUIRE_THROW(infoPlayerStart->SetCharMatrix("new", "AB", 1, 1), InvalidParameterException_c);

	//Matrix data does not match sizes
	BOOST_REQUIRE_THROW(infoPlayerStart->SetCharMatrix("matrix", "ABC", 1, 1), InvalidParameterException_c);

	//Cannot have size == 0 data
	BOOST_REQUIRE_THROW(infoPlayerStart->SetCharMatrix("inherit", "", 0, 0), InvalidParameterException_c);

	//Not a matrix type
	BOOST_REQUIRE_THROW(infoPlayerStart->GetMatrix("weight"), InvalidOperationException_c);

	//non existing
	BOOST_REQUIRE_THROW(infoPlayerStart->GetMatrix("bla"), ObjectNotFoundException_c);
}

BOOST_AUTO_TEST_CASE(dictionary_inheritance_setters)
{
	KernelInstance_s instance;

	stringstream stream;

	stream << "EntityDef InfoPlayerStart" << endl;
	stream << "{" << endl;
	stream <<	"className=Entity;" << endl;
	stream <<	"health=100;" << endl;
	stream <<	"weight=2.0;" << endl;
	stream <<	"description=\"bla bla\";" << endl;
	stream << "}" << endl;
	stream << endl << endl;
	stream << "OtherDef InfoPlayerStart" << endl;
	stream << "{" << endl;
	stream <<	"inherit=InfoPlayerStart;" << endl;
	stream <<	"health=200;" << endl;	
	stream <<	"boost=2;" << endl;	
	stream << "}";
	stream << "OtherDef Player" << endl;
	stream << "{" << endl;	
	stream << "}";

	DictionaryManagerPtr_t manager = DictionaryManager_c::GetInstance();

	manager->Load(stream);

	//test both forms of retrieving data	
	DictionaryPtr_t player = manager->GetDictionaryHive("OtherDef")->GetDictionary("Player");

	//now check overriding
	BOOST_REQUIRE_THROW(player->GetString("health"), ObjectNotFoundException_c);

	//Now check SetInherited
	player->SetInherited("InfoPlayerStart");
	BOOST_REQUIRE(player->GetString("health").compare("200") == 0);

	player->SetBaseHive("EntityDef");
	BOOST_REQUIRE(player->GetString("health").compare("100") == 0);

	//REstore original hive
	player->SetBaseHive("OtherDef");
	BOOST_REQUIRE(player->GetString("health").compare("200") == 0);	
}