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
#include <PH_Node.h>

using namespace boost;
using namespace Phobos;

static int iAliveNodes_gl = 0;

class TestNode_c: public Node_c
{
	public:
		TestNode_c(const Char_t *name):
			Node_c(name)
		{
			++iAliveNodes_gl;
		}

		~TestNode_c()
		{
			--iAliveNodes_gl;
		}

	private:	
};

typedef intrusive_ptr<TestNode_c> TestNodePtr_t;

BOOST_AUTO_TEST_CASE(node_basic)
{
	//Test the test :)
	{
		TestNodePtr_t ptr(new TestNode_c("root"));

		BOOST_REQUIRE(iAliveNodes_gl == 1);
	}

	BOOST_REQUIRE(iAliveNodes_gl == 0);

	//Now test basic child add / removal
	{
		TestNodePtr_t ptr(new TestNode_c("root"));
		BOOST_REQUIRE(ptr->GetNumChildren() == 0);
		BOOST_REQUIRE(!(ptr->HasChildren()));

		{
			TestNodePtr_t child(new TestNode_c("child0"));
			BOOST_REQUIRE(iAliveNodes_gl == 2);		

			ptr->AddChild(child);
			BOOST_REQUIRE(ptr->GetNumChildren() == 1);
			BOOST_REQUIRE(ptr->HasChildren());
			
			NodePtr_t parent(child->GetParent());
			BOOST_REQUIRE(parent);
			BOOST_REQUIRE(parent->GetName().compare("root") == 0);

			child = new TestNode_c("child1");
			BOOST_REQUIRE(iAliveNodes_gl == 3);
			ptr->AddChild(child);

			NodePtr_t retrievedPtr(ptr->GetChild("child0"));

			ptr->RemoveChild(child);
			child.reset();
		}

		NodePtr_t child(new TestNode_c("child2"));
		ptr->AddChild(child);
		child->RemoveSelf();
		child.reset();

		BOOST_REQUIRE(iAliveNodes_gl == 2);
	}

	BOOST_REQUIRE(iAliveNodes_gl == 0);
}

BOOST_AUTO_TEST_CASE(node_exceptions)
{
	//now check AddChild Exceptions
	{
		TestNodePtr_t ptr(new TestNode_c("root"));

		TestNodePtr_t child(new TestNode_c("child0"));
		TestNodePtr_t child1(new TestNode_c("child1"));

		ptr->AddChild(child);
		ptr->AddChild(child1);

		BOOST_REQUIRE_THROW( child->AddChild(child1), InvalidParameterException_c);

		//check remove child if a "virgin" node
		child1.reset(new TestNode_c("child0"));
		BOOST_REQUIRE_THROW(ptr->RemoveChild(child1), InvalidParameterException_c);

		BOOST_REQUIRE_THROW(ptr->AddChild(child1), ObjectAlreadyExistsException_c);

		//Check registered node, but not child of
		BOOST_REQUIRE_THROW(child1->RemoveChild(child), InvalidParameterException_c);

		BOOST_REQUIRE_THROW(child1->GetChild("bla"), ObjectNotFoundException_c);
	}

	
}
