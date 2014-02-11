/*
Phobos 3d
August 2013
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

#include <Phobos/HandleList.h>

class TestObject
{
	private:
		

	public:
		
		TestObject(const TestObject &rhs)
		{
			//Make sure it is never copied
			BOOST_REQUIRE(false);
		}

		TestObject &operator=(const TestObject &rhs)
		{
			//make sure it is never copied
			BOOST_REQUIRE(false);

			return *this;
		}

		TestObject &operator=(const TestObject &&rhs)
		{			
			return *this;
		}

		TestObject(TestObject &&rhs)
		{
			//empty
		}

		TestObject()
		{
			//empty
		}
};

using namespace Phobos;

BOOST_AUTO_TEST_CASE(list_basic)
{
	HandleList<TestObject> list;

	auto handler = list.Add(std::move(TestObject())).first;

	BOOST_REQUIRE(handler.GetSerial() == 1);
	BOOST_REQUIRE(handler.GetIndex() == 0);

	auto handler2 = list.Add(std::move(TestObject())).first;

	BOOST_REQUIRE(handler2.GetSerial() == 1);
	BOOST_REQUIRE(handler2.GetIndex() == 1);

	list.Remove(handler);

	BOOST_REQUIRE_THROW(list.Get(handler), ObjectNotFoundException);

	BOOST_REQUIRE_THROW(list.Get(Handle(1024, 1)), InvalidParameterException);

	//Add again, check re-use
	handler = list.Add(std::move(TestObject())).first;

	BOOST_REQUIRE(handler.GetSerial() == 2);
	BOOST_REQUIRE(handler.GetIndex() == 0);

	list.Clear();

	BOOST_REQUIRE_THROW(list.Get(handler), InvalidParameterException);
	BOOST_REQUIRE_THROW(list.Get(handler2), InvalidParameterException);
	
	//make sure list released memory
	handler = list.Add(std::move(TestObject())).first;

	BOOST_REQUIRE(handler.GetSerial() == 1);
	BOOST_REQUIRE(handler.GetIndex() == 0);
}
