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

#include <string>

#include <boost/filesystem.hpp>

#include <PH_Log.h>

#include "SimpleLogListener.h"

using namespace boost::filesystem;
using namespace std;

using namespace Phobos;

static const string LOG_FILE_NAME("logTest.log");

BOOST_AUTO_TEST_CASE(log_basic)
{	
	if(exists(LOG_FILE_NAME))
		BOOST_REQUIRE(remove(LOG_FILE_NAME));
	
	{
		Log_c log(LOG_FILE_NAME);

		BOOST_REQUIRE(exists(LOG_FILE_NAME));
		BOOST_REQUIRE(file_size(LOG_FILE_NAME) == 0);

		log.Message("Test");

		BOOST_REQUIRE(file_size(LOG_FILE_NAME) > 0);
	}

	{
		Log_c log(LOG_FILE_NAME);
		
		BOOST_REQUIRE(file_size(LOG_FILE_NAME) == 0);
	}
}

BOOST_AUTO_TEST_CASE(log_listener)
{
	Log_c log(LOG_FILE_NAME);

	BOOST_CHECK(log.GetListenersCount() == 0);	

	{
		SimpleLogListener_c a;

		log.AddListener(a);
		BOOST_CHECK(a.oFlag == false);
		BOOST_CHECK(log.GetListenersCount() == 1);

		log.Message("test");
		BOOST_CHECK(a.oFlag == true);
	}

	BOOST_CHECK(log.GetListenersCount() == 0);

	SimpleLogListener_c a, b;

	log.AddListener(a);
	log.Message("bla");
	BOOST_CHECK(a.oFlag == true);

	a.oFlag = false;
	log.AddListener(b);
	log.Message("bla");
	BOOST_CHECK(a.oFlag == true);
	BOOST_CHECK(b.oFlag == true);
}
