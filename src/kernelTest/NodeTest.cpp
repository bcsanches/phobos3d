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
#include <PH_Kernel.h>
#include <PH_Path.h>

using namespace boost;
using namespace Phobos;

static const String_c LOG_FILE_NAME("phobos.log");

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
		TestPrivateNode_c(const Char_t *name, UInt32_t flags = 0):
		  Node_c(name, flags)
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
	
	{
		TestNode_c root("root");
		BOOST_REQUIRE(root.GetNumChildren() == 0);
		BOOST_REQUIRE(!(root.HasChildren()));		

		{
			TestNode_c child0("child0");
			BOOST_REQUIRE(iAliveNodes_gl == 2);		

			root.AddChild(child0);
			BOOST_REQUIRE(root.GetNumChildren() == 1);
			BOOST_REQUIRE(root.HasChildren());
			
			Node_c *parent(child0.GetParent());
			BOOST_REQUIRE(parent);
			BOOST_REQUIRE(parent->GetName().compare("root") == 0);

			Path_c path;
			child0.GetThisPath(path);
			BOOST_REQUIRE(path.GetStr().compare("/root/child0")==0);

			TestNode_c child1("child1");
			BOOST_REQUIRE(iAliveNodes_gl == 3);
			root.AddChild(child1);

			Node_c &retrieved(root.GetChild("child0"));						
		}

		BOOST_REQUIRE(iAliveNodes_gl == 1);
		BOOST_REQUIRE(root.GetNumChildren() == 0);
		BOOST_REQUIRE(!(root.HasChildren()));

		{
			TestNode_c child("child2");
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
	Kernel_c &kernel = Kernel_c::CreateInstance(LOG_FILE_NAME);

	//now check AddChild Exceptions
	{
		TestNode_c root("root");

		TestNode_c child("child0");
		TestNode_c child1("child1");

		root.AddChild(child);
		root.AddChild(child1);

		BOOST_REQUIRE_THROW( child.AddChild(child1), InvalidParameterException_c);

		//check remove child if a "virgin" node
		TestNode_c virgin("child0");		
		BOOST_REQUIRE_THROW(root.RemoveChild(virgin), InvalidParameterException_c);

		BOOST_REQUIRE_THROW(root.AddChild(virgin), ObjectAlreadyExistsException_c);

		//Check registered node, but not child of
		BOOST_REQUIRE_THROW(virgin.RemoveChild(child), InvalidParameterException_c);

		BOOST_REQUIRE_THROW(virgin.GetChild("bla"), ObjectNotFoundException_c);
	}

	Kernel_c::ReleaseInstance();
}

BOOST_AUTO_TEST_CASE(node_private)
{
	Kernel_c &kernel = Kernel_c::CreateInstance(LOG_FILE_NAME);

	TestPrivateNode_c root("root_private_test", NodeFlags::PRIVATE_CHILDREN);
	
	TestPrivateNode_c child1("child_private_test");

	BOOST_REQUIRE_THROW(root.AddChild(child1), InvalidOperationException_c);

	Kernel_c::ReleaseInstance();
}

BOOST_AUTO_TEST_CASE(node_lookup)
{
	Kernel_c &kernel = Kernel_c::CreateInstance(LOG_FILE_NAME);	

	TestNode_c root("root");

	TestNode_c child("child0");
	TestNode_c child1("child1");
	TestNode_c child2("child2");

	root.AddChild(child);
	root.AddChild(child1);

	child1.AddChild(child2);

	BOOST_REQUIRE_THROW(root.LookupNode(Path_c("")), InvalidParameterException_c);

	BOOST_REQUIRE(child1.LookupNode(Path_c("/child0")).GetName().compare("child0") == 0);
	BOOST_REQUIRE(child1.LookupNode(Path_c("/")).GetName().compare("root") == 0);
	BOOST_REQUIRE(root.LookupNode(Path_c("/")).GetName().compare("root") == 0);

	BOOST_REQUIRE(root.LookupNode(Path_c("child1/child2")).GetName().compare("child2") == 0);
	BOOST_REQUIRE(child1.LookupNode(Path_c("child2")).GetName().compare("child2") == 0);

	BOOST_REQUIRE_THROW(root.LookupNode(Path_c("bla")), ObjectNotFoundException_c);


	//
	//Now check the "try" version
	//

	Node_c *result = NULL;

	BOOST_REQUIRE(!root.TryLookupNode(result, Path_c("")));
	BOOST_REQUIRE(!result);

	BOOST_REQUIRE(child1.TryLookupNode(result, Path_c("/child0")));
	BOOST_REQUIRE(result->GetName().compare("child0") == 0);

	BOOST_REQUIRE(child1.TryLookupNode(result, Path_c("/")));
	BOOST_REQUIRE(result->GetName().compare("root") == 0);

	BOOST_REQUIRE(root.TryLookupNode(result, Path_c("/")));
	BOOST_REQUIRE(result->GetName().compare("root") == 0);

	BOOST_REQUIRE(root.TryLookupNode(result, Path_c("child1/child2")));
	BOOST_REQUIRE(result->GetName().compare("child2") == 0);

	BOOST_REQUIRE(child1.TryLookupNode(result, Path_c("child2")));
	BOOST_REQUIRE(result->GetName().compare("child2") == 0);

	BOOST_REQUIRE(root.TryLookupNode(result, Path_c("bla")));
	BOOST_REQUIRE(!result);

	Kernel_c::ReleaseInstance();
}

BOOST_AUTO_TEST_CASE(node_addNode)
{
	Kernel_c &kernel = Kernel_c::CreateInstance(LOG_FILE_NAME);

	TestNode_c root("root");
	
	TestNode_c child("child0");

	root.AddNode(child, Path_c("children/"));
	BOOST_REQUIRE(root.LookupNode(Path_c("/children/child0")).GetName().compare("child0") == 0);
	BOOST_REQUIRE(root.LookupNode(Path_c("/children")).GetName().compare("children") == 0);

	TestNode_c child1("child1");
	root.AddNode(child1, Path_c("/"));
	BOOST_REQUIRE(root.LookupNode(Path_c("/child1")).GetName().compare("child1") == 0);

	Kernel_c::ReleaseInstance();
}
