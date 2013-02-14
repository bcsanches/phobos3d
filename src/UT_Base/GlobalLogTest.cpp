/*
Phobos 3d
January 2010
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

#include <Phobos/Log.h>

#include <boost/filesystem.hpp>

#include "SimpleLogListener.h"

using namespace boost::filesystem;

using namespace Phobos;

static const String_t DEFAULT_LOG_FILE_NAME("phobos.log");
static const String_t LOG_FILE_NAME("test.log");

BOOST_AUTO_TEST_CASE(kernel_basic)
{	
	if(exists(LOG_FILE_NAME))
		BOOST_REQUIRE(remove(LOG_FILE_NAME));

	BOOST_REQUIRE(!exists(LOG_FILE_NAME));

	LogChangeFile(LOG_FILE_NAME.c_str());	

	LogMessage("Test message");

	BOOST_REQUIRE(exists(LOG_FILE_NAME));
}

BOOST_AUTO_TEST_CASE(kernel_log_listener)
{
	SimpleLogListener a;

	LogAddListener(a);
	
	LogMessage("bla");

	BOOST_CHECK(a.oFlag == true);

	a.oFlag = false;

	LogRemoveListener(a);

	LogMessage("buuuu");
	BOOST_CHECK(a.oFlag == false);	
}
