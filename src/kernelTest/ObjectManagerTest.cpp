/*
Phobos 3d
  version 0.0.1, January 2010

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

#include <boost/test/unit_test.hpp>

#include <PH_Exception.h>
#include <PH_ObjectManager.h>
#include <PH_Path.h>

using namespace Phobos;

BOOST_AUTO_TEST_CASE(objectManager_basic)
{
	ObjectManager_c manager;	

	NodePtr_t node(Node_c::Create("test"));
	manager.AddObject(node, Path_c("sys/screen"));

	NodePtr_t sys = manager.LookupObject(Path_c("sys"));
	BOOST_REQUIRE(sys);
	BOOST_REQUIRE(sys->GetName().compare("sys") == 0);	

	NodePtr_t screen = manager.LookupObject(Path_c("sys/screen"));
	BOOST_REQUIRE(screen);
	BOOST_REQUIRE(screen->GetName().compare("screen") == 0);

	NodePtr_t parent = screen->GetParent();
	BOOST_REQUIRE(parent);
	BOOST_REQUIRE(parent->GetName().compare("sys")==0);
	
	BOOST_REQUIRE(manager.LookupObject(Path_c("sys/screen/test")));

	node = Node_c::Create("input");
	manager.AddObject(node, Path_c("/sys/"));

	NodePtr_t input = manager.LookupObject(Path_c("sys/input"));
	BOOST_REQUIRE(input);
	BOOST_REQUIRE(input->GetName().compare("input") == 0);

	node = Node_c::Create("test");
	manager.AddObject(node, Path_c());

	NodePtr_t test = manager.LookupObject(Path_c("test"));
	BOOST_REQUIRE(test);
	BOOST_REQUIRE(test->GetName().compare("test") == 0);
}

BOOST_AUTO_TEST_CASE(objectManager_exception)
{
	ObjectManager_c manager;

	BOOST_REQUIRE_THROW(manager.LookupObject(Path_c()), InvalidParameterException_c);
	BOOST_REQUIRE_THROW(manager.LookupObject(Path_c("bla/bla/bla")), ObjectNotFoundException_c);


}