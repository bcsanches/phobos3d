/*
Phobos 3d
  January 2010

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

#include <PH_Kernel.h>

#include <boost/filesystem.hpp>

#include "SimpleLogListener.h"

using namespace boost::filesystem;

using namespace Phobos;

static const String_c LOG_FILE_NAME("phobos.log");

BOOST_AUTO_TEST_CASE(kernel_basic)
{	
	if(exists(LOG_FILE_NAME))
		BOOST_REQUIRE(remove(LOG_FILE_NAME));

	Kernel_c &kernel = Kernel_c::CreateInstance(LOG_FILE_NAME);

	BOOST_REQUIRE(exists(LOG_FILE_NAME));

	Kernel_c::ReleaseInstance();
}

BOOST_AUTO_TEST_CASE(kernel_log_listener)
{
	SimpleLogListener_c a;

	Kernel_c &kernel = Kernel_c::CreateInstance(LOG_FILE_NAME);

	kernel.AddLogListener(a);
	kernel.LogMessage("bla");

	BOOST_CHECK(a.oFlag == true);

	a.oFlag = false;

	kernel.RemoveLogListener(a);

	kernel.LogMessage("buuuu");
	BOOST_CHECK(a.oFlag == false);
	
	Kernel_c::ReleaseInstance();
}