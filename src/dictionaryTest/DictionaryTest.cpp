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
	BOOST_REQUIRE(infoPlayerStart->GetValue("health").compare("100") == 0);
	BOOST_REQUIRE(superPlayer->GetValue("health").compare("200") == 0);

	//check parent relationship
	BOOST_REQUIRE(infoPlayerStart->GetValue("weight").compare("2.0") == 0);
	BOOST_REQUIRE(superPlayer->GetValue("weight").compare("2.0") == 0);

	//non existing value
	BOOST_REQUIRE_THROW(infoPlayerStart->GetValue("boost"), ObjectNotFoundException_c);
	BOOST_REQUIRE(superPlayer->GetValue("boost").compare("2") == 0);

	//bad inheritance
	DictionaryPtr_t invalidPlayer = manager->GetDictionary("EntityDef", "InvalidPlayer");

	BOOST_REQUIRE_THROW(infoPlayerStart->GetValue("bla"), ObjectNotFoundException_c);

	//Check getters exceptions
	BOOST_REQUIRE_THROW(manager->GetDictionaryHive("Bla"), ObjectNotFoundException_c);
	BOOST_REQUIRE_THROW(manager->GetDictionary("Bla", "Bla"), ObjectNotFoundException_c);
	BOOST_REQUIRE_THROW(manager->GetDictionary("EntityDef", "Bla"), ObjectNotFoundException_c);
}
