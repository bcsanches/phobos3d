/*
Phobos 3d
July 2011
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

#include <PH_HandleManager.h>

BOOST_AUTO_TEST_CASE(HandleManager_Full)
{
	using namespace Phobos;

	HandleManager_c<int> handleManager;

	//Fill it 
	Handle_s h;
	for(int i = 0;i < HandleManager_c<int>::MAX_ENTRIES; ++i)
	{
		h = handleManager.AddObject(reinterpret_cast<int *>(i));
	}

	//full, should fail
	BOOST_REQUIRE_THROW(handleManager.AddObject(reinterpret_cast<int *>(5)), InvalidOperationException_c);

	//remove last one
	handleManager.RemoveObject(h);

	//Add new one
	Handle_s h1 = handleManager.AddObject(reinterpret_cast<int *>(5));

	BOOST_REQUIRE(h1.u12Index == h.u12Index);
	BOOST_REQUIRE(h1.u20Counter != h.u20Counter);
}


BOOST_AUTO_TEST_CASE(HandleManagerBasic)
{
	using namespace Phobos;

	HandleManager_c<int> handleManager;

	Handle_s h1 = handleManager.AddObject(new int(1));
	Handle_s h2 = handleManager.AddObject(new int(2));

	//Check basic handle
	BOOST_REQUIRE(h1.u12Index == 0);
	BOOST_REQUIRE(h1.u20Counter == 1);

	BOOST_REQUIRE(h2.u12Index == 1);
	BOOST_REQUIRE(h2.u20Counter == 1);

	//Check get
	int *p = handleManager.TryGetObject(h1);
	BOOST_REQUIRE(*p == 1);

	//Check remove
	handleManager.RemoveObject(h1);
	BOOST_REQUIRE(handleManager.TryGetObject(h1) == NULL);

	//Check if slot re-used and counter changed
	h1 = handleManager.AddObject(p);
	BOOST_REQUIRE(h1.u12Index == 0);
	BOOST_REQUIRE(h1.u20Counter == 2);

	//Try invalid handle
	BOOST_REQUIRE(handleManager.TryGetObject(Handle_s(55, 123)) == NULL);
	BOOST_REQUIRE(handleManager.TryGetObject(Handle_s(0, 123)) == NULL);

	//Try remove invalid
	BOOST_REQUIRE_THROW(handleManager.RemoveObject(Handle_s(55, 123)), InvalidParameterException_c);
	BOOST_REQUIRE_THROW(handleManager.RemoveObject(Handle_s(0, 123)), InvalidParameterException_c);

	delete handleManager.TryGetObject(h1);
	delete handleManager.TryGetObject(h2);
}



