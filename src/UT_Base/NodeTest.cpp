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
#include <Phobos/Node.h>
#include <Phobos/Path.h>

using namespace Phobos;

static const String_t LOG_FILE_NAME("phobos.log");

static int iAliveNodes_gl = 0;

class TestNode: public Node
{
	public:
		TestNode(const Char_t *name):
			Node(name)
		{
			++iAliveNodes_gl;
		}

		~TestNode()
		{
			--iAliveNodes_gl;
		}

	private:	
};

class TestPrivateNode: public Node
{
	public:
		TestPrivateNode(const Char_t *name, UInt32_t flags = 0):
		  Node(name, flags)
		  {
		  }
};

PH_DECLARE_NODE_PTR(TestPrivateNode);
PH_DECLARE_NODE_PTR(TestNode);

BOOST_AUTO_TEST_CASE(node_basic)
{
	//Test the test :)	
	{
		TestNodePtr_t ptr(std::make_shared<TestNode>("root"));

		BOOST_REQUIRE(iAliveNodes_gl == 1);		
	}

	BOOST_REQUIRE(iAliveNodes_gl == 0);
	
	{
		TestNode root("root");
		BOOST_REQUIRE(root.GetNumChildren() == 0);
		BOOST_REQUIRE(!(root.HasChildren()));		

		{
			TestNode child0("child0");
			BOOST_REQUIRE(iAliveNodes_gl == 2);		

			root.AddChild(child0);
			BOOST_REQUIRE(root.GetNumChildren() == 1);
			BOOST_REQUIRE(root.HasChildren());
			
			Node *parent(child0.GetParent());
			BOOST_REQUIRE(parent);
			BOOST_REQUIRE(parent->GetName().compare("root") == 0);

			Path path;
			child0.GetThisPath(path);
			BOOST_REQUIRE(path.GetStr().compare("/root/child0")==0);

			TestNode child1("child1");
			BOOST_REQUIRE(iAliveNodes_gl == 3);
			root.AddChild(child1);

			Node &retrieved(root.GetChild("child0"));						
		}

		BOOST_REQUIRE(iAliveNodes_gl == 1);
		BOOST_REQUIRE(root.GetNumChildren() == 0);
		BOOST_REQUIRE(!(root.HasChildren()));

		{
			TestNode child("child2");
			root.AddChild(child);
			child.RemoveSelf();

			BOOST_REQUIRE(root.GetNumChildren() == 0);
			BOOST_REQUIRE(!(root.HasChildren()));
		}

		BOOST_REQUIRE(!(root.TryGetChild("child2")));

		BOOST_REQUIRE(iAliveNodes_gl == 1);
	}

	BOOST_REQUIRE(iAliveNodes_gl == 0);
}

BOOST_AUTO_TEST_CASE(node_exceptions)
{	
	//now check AddChild Exceptions
	{
		TestNode root("root");

		TestNode child("child0");
		TestNode child1("child1");

		root.AddChild(child);
		root.AddChild(child1);

		BOOST_REQUIRE_THROW( child.AddChild(child1), InvalidParameterException);

		//check remove child if a "virgin" node
		TestNode virgin("child0");		
		BOOST_REQUIRE_THROW(root.RemoveChild(virgin), InvalidParameterException);

		BOOST_REQUIRE_THROW(root.AddChild(virgin), ObjectAlreadyExistsException);

		//Check registered node, but not child of
		BOOST_REQUIRE_THROW(virgin.RemoveChild(child), InvalidParameterException);

		BOOST_REQUIRE_THROW(virgin.GetChild("bla"), ObjectNotFoundException);
	}	
}

BOOST_AUTO_TEST_CASE(node_private)
{	
	TestPrivateNode root("root_private_test", NodeFlags::PRIVATE_CHILDREN);
	
	TestPrivateNode child1("child_private_test");

	BOOST_REQUIRE_THROW(root.AddChild(child1), InvalidOperationException);	
}

BOOST_AUTO_TEST_CASE(node_lookup)
{	
	TestNode root("root");

	TestNode child("child0");
	TestNode child1("child1");
	TestNode child2("child2");

	root.AddChild(child);
	root.AddChild(child1);

	child1.AddChild(child2);

	BOOST_REQUIRE_THROW(root.LookupNode(Path("")), InvalidParameterException);

	BOOST_REQUIRE(child1.LookupNode(Path("/child0")).GetName().compare("child0") == 0);
	BOOST_REQUIRE(child1.LookupNode(Path("/")).GetName().compare("root") == 0);
	BOOST_REQUIRE(root.LookupNode(Path("/")).GetName().compare("root") == 0);

	BOOST_REQUIRE(root.LookupNode(Path("child1/child2")).GetName().compare("child2") == 0);
	BOOST_REQUIRE(child1.LookupNode(Path("child2")).GetName().compare("child2") == 0);

	BOOST_REQUIRE_THROW(root.LookupNode(Path("bla")), ObjectNotFoundException);


	//
	//Now check the "try" version
	//

	Node *result = NULL;

	BOOST_REQUIRE(!root.TryLookupNode(result, Path("")));
	BOOST_REQUIRE(!result);

	BOOST_REQUIRE(child1.TryLookupNode(result, Path("/child0")));
	BOOST_REQUIRE(result->GetName().compare("child0") == 0);

	BOOST_REQUIRE(child1.TryLookupNode(result, Path("/")));
	BOOST_REQUIRE(result->GetName().compare("root") == 0);

	BOOST_REQUIRE(root.TryLookupNode(result, Path("/")));
	BOOST_REQUIRE(result->GetName().compare("root") == 0);

	BOOST_REQUIRE(root.TryLookupNode(result, Path("child1/child2")));
	BOOST_REQUIRE(result->GetName().compare("child2") == 0);

	BOOST_REQUIRE(child1.TryLookupNode(result, Path("child2")));
	BOOST_REQUIRE(result->GetName().compare("child2") == 0);

	BOOST_REQUIRE(root.TryLookupNode(result, Path("bla")));
	BOOST_REQUIRE(!result);	
}

BOOST_AUTO_TEST_CASE(node_addNode)
{	
	TestNode root("root");
	
	TestNode child("child0");

	root.AddNode(child, Path("children/"));
	BOOST_REQUIRE(root.LookupNode(Path("/children/child0")).GetName().compare("child0") == 0);
	BOOST_REQUIRE(root.LookupNode(Path("/children")).GetName().compare("children") == 0);

	TestNode child1("child1");
	root.AddNode(child1, Path("/"));
	BOOST_REQUIRE(root.LookupNode(Path("/child1")).GetName().compare("child1") == 0);	
}
