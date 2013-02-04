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


#include <boost/test/unit_test.hpp>

#include <Phobos/Register/Table.h>
#include <Phobos/Register/Hive.h>
#include <Phobos/Register/Manager.h>

#include <Phobos/Exception.h>

using namespace Phobos;
using namespace std;

struct KernelInstance_s
{
	KernelInstance_s()
	{		
		Register::Init();
	}

	~KernelInstance_s()
	{		
		Register::Finalize();		
	}
};

BOOST_AUTO_TEST_CASE(register_basic)
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
	stream << "RigidBody" << endl; //no name test
	stream << "{" << endl;
	stream <<	"height=1.0;" << endl;
	stream <<	"radius=2.0;" << endl;
	stream << "}";

	Register::Load(stream);	
}

BOOST_AUTO_TEST_CASE(dictionary_errors)
{
	KernelInstance_s instance;	

	{
		//invalid syntax
		stringstream stream;
		
		stream << "{" << endl;

		BOOST_REQUIRE_THROW(Register::Load(stream), ParserException);
	}

	{
		//invalid syntax
		stringstream stream;
		
		//missing hive
		stream << "EntityDef " << endl;
		stream << "{" << endl;

		BOOST_REQUIRE_THROW(Register::Load(stream), ParserException);
	}

	{
		//invalid syntax
		stringstream stream;
		
		stream << "EntityDef InfoPlayerStart" << endl;
		stream << "{" << endl;
		//missing =
		stream <<	"className Entity" << endl;

		BOOST_REQUIRE_THROW(Register::Load(stream), ParserException);
	}

	{
		//invalid syntax
		stringstream stream;
		
		stream << "EntityDef InfoPlayerStart" << endl;
		stream << "{" << endl;
		//missing ;
		stream <<	"className=Entity" << endl;
		stream <<	"health=100;" << endl;

		BOOST_REQUIRE_THROW(Register::Load(stream), ParserException);
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

		BOOST_REQUIRE_THROW(Register::Load(stream), ObjectAlreadyExistsException);
	}
}

BOOST_AUTO_TEST_CASE(register_inheritance)
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

	Register::Load(stream);

	//test both forms of retrieving data
	auto &infoPlayerStart = Register::GetTable("EntityDef", "InfoPlayerStart");
	auto &superPlayer = Register::GetHive("EntityDef").GetTable("SuperPlayer");	

	//now check overriding
	BOOST_REQUIRE(infoPlayerStart.GetString("health").compare("100") == 0);
	BOOST_REQUIRE(superPlayer.GetString("health").compare("200") == 0);

	//check parent relationship
	BOOST_REQUIRE(infoPlayerStart.GetString("weight").compare("2.0") == 0);
	BOOST_REQUIRE(superPlayer.GetString("weight").compare("2.0") == 0);

	//non existing value
	BOOST_REQUIRE_THROW(infoPlayerStart.GetString("boost"), ObjectNotFoundException);
	BOOST_REQUIRE(superPlayer.GetString("boost").compare("2") == 0);

	//bad inheritance
	auto &invalidPlayer = Register::GetTable("EntityDef", "InvalidPlayer");

	BOOST_REQUIRE_THROW(infoPlayerStart.GetString("bla"), ObjectNotFoundException);

	//Check getters exceptions
	BOOST_REQUIRE_THROW(Register::GetHive("Bla"), ObjectNotFoundException);
	BOOST_REQUIRE_THROW(Register::GetTable("Bla", "Bla"), ObjectNotFoundException);
	BOOST_REQUIRE_THROW(Register::GetTable("EntityDef", "Bla"), ObjectNotFoundException);

	BOOST_REQUIRE_THROW(infoPlayerStart.SetString("new", "invalid keyword"), InvalidParameterException);
}

/**
	The same as dictionary_inheritance, but the inheritance chain uses a baseHive

*/
BOOST_AUTO_TEST_CASE(register_inheritance_other_hive)
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

	Register::Load(stream);

	//test both forms of retrieving data
	auto &infoPlayerStart = Register::GetTable("EntityDef", "InfoPlayerStart");
	auto &superPlayer = Register::GetHive("DynamicEntity").GetTable("SuperPlayer");

	//now check overriding
	BOOST_REQUIRE(infoPlayerStart.GetString("health").compare("100") == 0);
	BOOST_REQUIRE(superPlayer.GetString("health").compare("200") == 0);

	//check parent relationship
	BOOST_REQUIRE(infoPlayerStart.GetString("weight").compare("2.0") == 0);
	BOOST_REQUIRE(superPlayer.GetString("weight").compare("2.0") == 0);

	//non existing value
	BOOST_REQUIRE_THROW(infoPlayerStart.GetString("boost"), ObjectNotFoundException);
	BOOST_REQUIRE(superPlayer.GetString("boost").compare("2") == 0);

	//bad inheritance
	auto &invalidPlayer = Register::GetTable("DynamicEntity", "InvalidPlayer");

	BOOST_REQUIRE_THROW(infoPlayerStart.GetString("bla"), ObjectNotFoundException);	
}

BOOST_AUTO_TEST_CASE(register_parse_matrix)
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

	Register::Load(stream);

	auto &world = Register::GetTable("StaticEntities", "World");

	auto handle = world.GetMatrix("map");

	BOOST_REQUIRE(handle(1, 1) == '.');
	BOOST_REQUIRE(handle(4, 3) == '.');

	BOOST_REQUIRE(handle.GetNumColumns() == 8);
	BOOST_REQUIRE(handle.GetNumRows() == 9);
}

BOOST_AUTO_TEST_CASE(register_parse_matrix_errors)
{
	KernelInstance_s instance;

	{
		//Invalid special type
		stringstream stream;

		stream << "StaticEntities World" << endl;
		stream << "{" << endl;
		stream <<	"map = new Bla;" << endl;
		stream << "}";

		BOOST_REQUIRE_THROW(Register::Load(stream), ParserException);
	}


	{
		//Invalid token type after new
		stringstream stream;

		stream << "StaticEntities World" << endl;
		stream << "{" << endl;
		stream <<	"map = new 123;" << endl;
		stream << "}";		

		BOOST_REQUIRE_THROW(Register::Load(stream), ParserException);
	}		

	{
		//Expected ( after CharMatrix
		stringstream stream;

		stream << "StaticEntities World" << endl;
		stream << "{" << endl;
		stream <<	"map = new CharMatrix;" << endl;
		stream << "}";		

		BOOST_REQUIRE_THROW(Register::Load(stream), ParserException);
	}

	{
		//Expected () data for CharMatrix
		stringstream stream;

		stream << "StaticEntities World" << endl;
		stream << "{" << endl;
		stream <<	"map = new CharMatrix();" << endl;
		stream << "}";		

		BOOST_REQUIRE_THROW(Register::Load(stream), ParserException);
	}

	{
		//CharMatrix data cannot be empty
		stringstream stream;

		stream << "StaticEntities World" << endl;
		stream << "{" << endl;
		stream <<	"map = new CharMatrix(\"\");" << endl;
		stream << "}";		

		BOOST_REQUIRE_THROW(Register::Load(stream), ParserException);
	}

	{
		//CharMatrix should be simetric
		stringstream stream;

		stream << "StaticEntities World" << endl;
		stream << "{" << endl;
		stream <<	"map = new CharMatrix(\"123\",\"12\");" << endl;
		stream << "}";		

		BOOST_REQUIRE_THROW(Register::Load(stream), ParserException);
	}

	{
		//CharMatrix should be formed by strings
		stringstream stream;

		stream << "StaticEntities World" << endl;
		stream << "{" << endl;
		stream <<	"map = new CharMatrix(1223);" << endl;
		stream << "}";		

		BOOST_REQUIRE_THROW(Register::Load(stream), ParserException);
	}
}

BOOST_AUTO_TEST_CASE(register_add_matrix_errors)
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

	Register::Load(stream);

	auto &infoPlayerStart = Register::GetTable("EntityDef", "InfoPlayerStart");

	//Cannot use inherit as matrix
	BOOST_REQUIRE_THROW(infoPlayerStart.SetCharMatrix("inherit", "AB", 1, 1), InvalidParameterException);
	BOOST_REQUIRE_THROW(infoPlayerStart.SetCharMatrix("base_hive", "AB", 1, 1), InvalidParameterException);
	BOOST_REQUIRE_THROW(infoPlayerStart.SetCharMatrix("new", "AB", 1, 1), InvalidParameterException);

	//Matrix data does not match sizes
	BOOST_REQUIRE_THROW(infoPlayerStart.SetCharMatrix("matrix", "ABC", 1, 1), InvalidParameterException);

	//Cannot have size == 0 data
	BOOST_REQUIRE_THROW(infoPlayerStart.SetCharMatrix("inherit", "", 0, 0), InvalidParameterException);

	//Not a matrix type
	BOOST_REQUIRE_THROW(infoPlayerStart.GetMatrix("weight"), InvalidOperationException);

	//non existing
	BOOST_REQUIRE_THROW(infoPlayerStart.GetMatrix("bla"), ObjectNotFoundException);
}

BOOST_AUTO_TEST_CASE(register_inheritance_setters)
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

	Register::Load(stream);

	//test both forms of retrieving data	
	auto &player = Register::GetHive("OtherDef").GetTable("Player");

	//now check overriding
	BOOST_REQUIRE_THROW(player.GetString("health"), ObjectNotFoundException);

	//Now check SetInherited
	player.SetInherited("InfoPlayerStart");
	BOOST_REQUIRE(player.GetString("health").compare("200") == 0);

	player.SetBaseHive("EntityDef");
	BOOST_REQUIRE(player.GetString("health").compare("100") == 0);

	//REstore original hive
	player.SetBaseHive("OtherDef");
	BOOST_REQUIRE(player.GetString("health").compare("200") == 0);	
}

BOOST_AUTO_TEST_CASE(register_auto_inheritance)
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
	stream << "EntityDef SuperPlayerStart: InfoPlayerStart" << endl;
	stream << "{" << endl;	
	stream <<	"health=200;" << endl;	
	stream <<	"boost=2;" << endl;	
	stream << "}";
	stream << "OtherDef Player: EntityDef.InfoPlayerStart" << endl;
	stream << "{" << endl;	
	stream << "}";

	Register::Load(stream);

	//test both forms of retrieving data	
	auto &player = Register::GetHive("OtherDef").GetTable("Player");

	//now check overriding
	BOOST_REQUIRE(player.GetString("health").compare("100") == 0);

	auto &superPlayer = Register::GetHive("EntityDef").GetTable("SuperPlayerStart");
	BOOST_REQUIRE(superPlayer.GetString("health").compare("200") == 0);	
}


BOOST_AUTO_TEST_CASE(register_auto_inheritance_parser_variations)
{
	KernelInstance_s instance;

	stringstream stream;

	stream << "EntityDef InfoPlayerStart" << endl;
	stream << "{" << endl;	
	stream << "}" << endl;
	stream << endl << endl;
	stream << "EntityDef SuperPlayerStart: InfoPlayerStart" << endl;
	stream << "{" << endl;		
	stream << "}";
	//nameless
	stream << "EntityDef : EntityDef.InfoPlayerStart" << endl;
	stream << "{" << endl;	
	stream << "}";
	//nameless twice to catch a bug when DOT is not correct handled
	stream << "EntityDef : EntityDef.InfoPlayerStart" << endl;
	stream << "{" << endl;	
	stream << "}";

	//nameless
	stream << "EntityDef : InfoPlayerStart" << endl;
	stream << "{" << endl;	
	stream << "}";

	//nameless
	stream << "EntityDef " << endl;
	stream << "{" << endl;	
	stream << "}";

	//only parsing
	Register::Load(stream);	
}

BOOST_AUTO_TEST_CASE(register_auto_inheritance_parser_errors)
{
	KernelInstance_s instance;

	{
		stringstream stream;

		stream << "EntityDef :2" << endl;
		stream << "{" << endl;	
		stream << "}" << endl;		

		BOOST_REQUIRE_THROW(Register::Load(stream), ParserException);
	}

	{
		stringstream stream;

		stream << "EntityDef : abc." << endl;
		stream << "{" << endl;	
		stream << "}" << endl;		

		BOOST_REQUIRE_THROW(Register::Load(stream), ParserException);
	}

	{
		stringstream stream;

		stream << "EntityDef : abc abc" << endl;
		stream << "{" << endl;	
		stream << "}" << endl;		

		BOOST_REQUIRE_THROW(Register::Load(stream), ParserException);
	}
}
