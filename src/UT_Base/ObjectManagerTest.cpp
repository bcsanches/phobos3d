/*
Phobos 3d
January 2010
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

#include <Phobos/Exception.h>
#include <Phobos/ObjectManager.h>
#include <Phobos/Path.h>

using namespace Phobos;

BOOST_AUTO_TEST_CASE(objectManager_basic)
{
	BOOST_REQUIRE(ObjectManager::IsEmpty());

	Node test("test");
	ObjectManager::AddObject(test, Path("sys/screen"));

	Node &sys = ObjectManager::LookupObject(Path("sys"));	
	BOOST_REQUIRE(sys.GetName().compare("sys") == 0);	

	Node &screen = ObjectManager::LookupObject(Path("sys/screen"));	
	BOOST_REQUIRE(screen.GetName().compare("screen") == 0);

	Node *parent = screen.GetParent();
	BOOST_REQUIRE(parent);
	BOOST_REQUIRE(parent->GetName().compare("sys")==0);
	
	BOOST_REQUIRE(ObjectManager::LookupObject(Path("sys/screen/test")).GetName().compare("test") == 0);

	Node input("input");
	ObjectManager::AddObject(input, Path("/sys/"));

	Node &inputLookup = ObjectManager::LookupObject(Path("sys/input"));	
	BOOST_REQUIRE(&inputLookup == &input);
	BOOST_REQUIRE(inputLookup.GetName().compare("input") == 0);

	Node test2("test");
	ObjectManager::AddObject(test2, Path());

	Node &test3 = ObjectManager::LookupObject(Path("test"));	
	BOOST_REQUIRE(test3.GetName().compare("test") == 0);

	ObjectManager::Clear();
}

BOOST_AUTO_TEST_CASE(objectManager_exception)
{	
	BOOST_REQUIRE(ObjectManager::IsEmpty());

	BOOST_REQUIRE_THROW(ObjectManager::LookupObject(Path()), InvalidParameterException);
	BOOST_REQUIRE_THROW(ObjectManager::LookupObject(Path("bla/bla/bla")), ObjectNotFoundException);	
}
