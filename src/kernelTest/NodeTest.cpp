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

#include <boost/make_shared.hpp>

#include <PH_Exception.h>
#include <PH_Node.h>
#include <PH_Path.h>

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

class TestPrivateNode_c: public Node_c
{
	public:
		TestPrivateNode_c(const Char_t *name, ChildrenMode_e mode=PUBLIC_CHILDREN):
		  Node_c(name, mode)
		  {
		  }
};

PH_DECLARE_NODE_PTR(TestPrivateNode);
PH_DECLARE_NODE_PTR(TestNode);

BOOST_AUTO_TEST_CASE(node_basic)
{
	//Test the test :)	
	{
		TestNodePtr_t ptr(boost::make_shared<TestNode_c>("root"));

		BOOST_REQUIRE(iAliveNodes_gl == 1);		
	}

	BOOST_REQUIRE(iAliveNodes_gl == 0);

	//Now test basic child add / removal
	{
		TestNodePtr_t ptr(boost::make_shared<TestNode_c>("root"));
		BOOST_REQUIRE(ptr->GetNumChildren() == 0);
		BOOST_REQUIRE(!(ptr->HasChildren()));

		{
			TestNodePtr_t child(boost::make_shared<TestNode_c>("child0"));
			BOOST_REQUIRE(iAliveNodes_gl == 2);		

			ptr->AddChild(child);
			BOOST_REQUIRE(ptr->GetNumChildren() == 1);
			BOOST_REQUIRE(ptr->HasChildren());
			
			NodePtr_t parent(child->GetParent());
			BOOST_REQUIRE(parent);
			BOOST_REQUIRE(parent->GetName().compare("root") == 0);

			Path_c path;
			child->GetThisPath(path);
			BOOST_REQUIRE(path.GetStr().compare("/root/child0")==0);

			child = TestNodePtr_t(boost::make_shared<TestNode_c>("child1"));
			BOOST_REQUIRE(iAliveNodes_gl == 3);
			ptr->AddChild(child);

			NodePtr_t retrievedPtr(ptr->GetChild("child0"));

			ptr->RemoveChild(child);
			child.reset();
		}

		NodePtr_t child(boost::make_shared<TestNode_c>("child2"));
		ptr->AddChild(child);
		child->RemoveSelf();
		child.reset();

		BOOST_REQUIRE(!(ptr->TryGetChild("child2")));

		BOOST_REQUIRE(iAliveNodes_gl == 2);
	}

	BOOST_REQUIRE(iAliveNodes_gl == 0);
}

BOOST_AUTO_TEST_CASE(node_exceptions)
{
	//now check AddChild Exceptions
	{
		TestNodePtr_t ptr(boost::make_shared<TestNode_c>("root"));

		TestNodePtr_t child(boost::make_shared< TestNode_c>("child0"));
		TestNodePtr_t child1(boost::make_shared< TestNode_c>("child1"));

		ptr->AddChild(child);
		ptr->AddChild(child1);

		BOOST_REQUIRE_THROW( child->AddChild(child1), InvalidParameterException_c);

		//check remove child if a "virgin" node
		child1.swap(boost::make_shared<TestNode_c>("child0"));
		BOOST_REQUIRE_THROW(ptr->RemoveChild(child1), InvalidParameterException_c);

		BOOST_REQUIRE_THROW(ptr->AddChild(child1), ObjectAlreadyExistsException_c);

		//Check registered node, but not child of
		BOOST_REQUIRE_THROW(child1->RemoveChild(child), InvalidParameterException_c);

		BOOST_REQUIRE_THROW(child1->GetChild("bla"), ObjectNotFoundException_c);
	}

	
}

BOOST_AUTO_TEST_CASE(node_private)
{
	TestPrivateNodePtr_t ptr(boost::make_shared<TestPrivateNode_c>("root_private_test", PRIVATE_CHILDREN));
	
	TestPrivateNodePtr_t child1(boost::make_shared<TestPrivateNode_c>("child_private_test", PUBLIC_CHILDREN));

	BOOST_REQUIRE_THROW(ptr->AddChild(child1), InvalidOperationException_c);
}

BOOST_AUTO_TEST_CASE(node_lookup)
{
	TestNodePtr_t ptr(boost::make_shared<TestNode_c>("root"));

	TestNodePtr_t child(boost::make_shared< TestNode_c>("child0"));
	TestNodePtr_t child1(boost::make_shared< TestNode_c>("child1"));
	TestNodePtr_t child2(boost::make_shared< TestNode_c>("child2"));

	ptr->AddChild(child);
	ptr->AddChild(child1);

	child1->AddChild(child2);

	BOOST_REQUIRE_THROW(ptr->LookupNode(Path_c("")), InvalidParameterException_c);

	BOOST_REQUIRE(child1->LookupNode(Path_c("/child0"))->GetName().compare("child0") == 0);
	BOOST_REQUIRE(child1->LookupNode(Path_c("/"))->GetName().compare("root") == 0);
	BOOST_REQUIRE(ptr->LookupNode(Path_c("/"))->GetName().compare("root") == 0);

	BOOST_REQUIRE(ptr->LookupNode(Path_c("child1/child2"))->GetName().compare("child2") == 0);
	BOOST_REQUIRE(child1->LookupNode(Path_c("child2"))->GetName().compare("child2") == 0);

	BOOST_REQUIRE_THROW(ptr->LookupNode(Path_c("bla")), ObjectNotFoundException_c);


	//
	//Now check the "try" version
	//

	NodePtr_t result;

	BOOST_REQUIRE(!ptr->TryLookupNode(result, Path_c("")));
	BOOST_REQUIRE(!result);

	BOOST_REQUIRE(child1->TryLookupNode(result, Path_c("/child0")));
	BOOST_REQUIRE(result->GetName().compare("child0") == 0);

	BOOST_REQUIRE(child1->TryLookupNode(result, Path_c("/")));
	BOOST_REQUIRE(result->GetName().compare("root") == 0);

	BOOST_REQUIRE(ptr->TryLookupNode(result, Path_c("/")));
	BOOST_REQUIRE(result->GetName().compare("root") == 0);

	BOOST_REQUIRE(ptr->TryLookupNode(result, Path_c("child1/child2")));
	BOOST_REQUIRE(result->GetName().compare("child2") == 0);

	BOOST_REQUIRE(child1->TryLookupNode(result, Path_c("child2")));
	BOOST_REQUIRE(result->GetName().compare("child2") == 0);

	BOOST_REQUIRE(ptr->TryLookupNode(result, Path_c("bla")));
	BOOST_REQUIRE(!result);
}

BOOST_AUTO_TEST_CASE(node_addNode)
{
	TestNodePtr_t ptr(boost::make_shared< TestNode_c>("root"));
	
	TestNodePtr_t child(boost::make_shared< TestNode_c>("child0"));

	ptr->AddNode(child, Path_c("children/"));
	BOOST_REQUIRE(ptr->LookupNode(Path_c("/children/child0"))->GetName().compare("child0") == 0);
	BOOST_REQUIRE(ptr->LookupNode(Path_c("/children"))->GetName().compare("children") == 0);

	TestNodePtr_t child1(boost::make_shared< TestNode_c>("child1"));
	ptr->AddNode(child1, Path_c("/"));
	BOOST_REQUIRE(ptr->LookupNode(Path_c("/child1"))->GetName().compare("child1") == 0);
}
