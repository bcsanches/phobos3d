/*
Phobos 3d
March 2010
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

#include <PH_ContextVar.h>
#include <PH_Exception.h>

using namespace Phobos;

static bool fLocalVarCallbackCalled = false;

static void LocalVarCallback(const class ContextVar_c &var, const String_c &oldValue, const String_c &newValue)
{
	fLocalVarCallbackCalled = true;
}

static void LocalVarThrowCallback(const class ContextVar_c &var, const String_c &oldValue, const String_c &newValue)
{
	PH_RAISE(INVALID_OPERATION_EXCEPTION, "LocalVarThrowCallback", "Teting");
}

BOOST_AUTO_TEST_CASE(contextVar_basic)
{
	ContextVar_c a("test", "5");

	BOOST_REQUIRE(a.GetInt() == 5);

	a.SetValue("10");
	BOOST_REQUIRE(a.GetInt() == 10);

	a.SetCallback(LocalVarCallback);
	a.SetValue("bla");

	BOOST_REQUIRE(fLocalVarCallbackCalled);

	ContextVar_c b("test2", "0", LocalVarThrowCallback);

	BOOST_REQUIRE(!b.GetBoolean());
	BOOST_REQUIRE_THROW(b.SetValue("true"), InvalidOperationException_c);

	b.SetCallback(VarCallback_t());
	b.SetValue("true");

	BOOST_REQUIRE(b.GetBoolean());
}
