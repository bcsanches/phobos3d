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

#include <PH_Exception.h>
#include <PH_ObjectManager.h>
#include <PH_Kernel.h>
#include <PH_Path.h>

using namespace Phobos;

static const String_c LOG_FILE_NAME("phobos.log");

BOOST_AUTO_TEST_CASE(objectManager_basic)
{
	ObjectManager_c manager;	

	Node_c test("test");
	manager.AddObject(test, Path_c("sys/screen"));

	Node_c &sys = manager.LookupObject(Path_c("sys"));	
	BOOST_REQUIRE(sys.GetName().compare("sys") == 0);	

	Node_c &screen = manager.LookupObject(Path_c("sys/screen"));	
	BOOST_REQUIRE(screen.GetName().compare("screen") == 0);

	Node_c *parent = screen.GetParent();
	BOOST_REQUIRE(parent);
	BOOST_REQUIRE(parent->GetName().compare("sys")==0);
	
	BOOST_REQUIRE(manager.LookupObject(Path_c("sys/screen/test")).GetName().compare("test") == 0);

	Node_c input("input");
	manager.AddObject(input, Path_c("/sys/"));

	Node_c &inputLookup = manager.LookupObject(Path_c("sys/input"));	
	BOOST_REQUIRE(&inputLookup == &input);
	BOOST_REQUIRE(inputLookup.GetName().compare("input") == 0);

	Node_c test2("test");
	manager.AddObject(test2, Path_c());

	Node_c &test3 = manager.LookupObject(Path_c("test"));	
	BOOST_REQUIRE(test3.GetName().compare("test") == 0);
}

BOOST_AUTO_TEST_CASE(objectManager_exception)
{
	Kernel_c &kernel = Kernel_c::CreateInstance(LOG_FILE_NAME);	

	ObjectManager_c manager;

	BOOST_REQUIRE_THROW(manager.LookupObject(Path_c()), InvalidParameterException_c);
	BOOST_REQUIRE_THROW(manager.LookupObject(Path_c("bla/bla/bla")), ObjectNotFoundException_c);

	Kernel_c::ReleaseInstance();
}