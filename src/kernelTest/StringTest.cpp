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

#include <PH_String.h>

#define TRIM_STR		"a b c"
#define RIGHT_TRIM_STR	"a b c  "
#define LEFT_TRIM_STR	"  a b c"
#define BOTH_TRIM_STR	"  a b c  "

#define BLANK_STRING "    "

using namespace Phobos;

BOOST_AUTO_TEST_CASE(string_trim)
{
	String_c str(RIGHT_TRIM_STR);
			
	StringTrim(str, STRING_TRIM_LEFT);
	BOOST_REQUIRE(str.compare(RIGHT_TRIM_STR) == 0);
	
	StringTrim(str, STRING_TRIM_RIGHT);
	BOOST_REQUIRE(str.compare(TRIM_STR) == 0);
	
	StringTrim(str, STRING_TRIM_BOTH);
	BOOST_CHECK(str.compare(TRIM_STR) == 0);

	str = "";
	StringTrim(str, STRING_TRIM_BOTH);
	BOOST_CHECK(str.compare("") == 0);

	str = LEFT_TRIM_STR;
	
	StringTrim(str, STRING_TRIM_RIGHT);
	BOOST_REQUIRE(str.compare(LEFT_TRIM_STR) == 0);
	
	StringTrim(str, STRING_TRIM_LEFT);
	BOOST_REQUIRE(str.compare(TRIM_STR) == 0);

	
	StringTrim(str, STRING_TRIM_BOTH);
	BOOST_CHECK(str.compare(TRIM_STR) == 0);

	str = BOTH_TRIM_STR;
	StringTrim(str, STRING_TRIM_RIGHT);
	BOOST_REQUIRE(str.compare(LEFT_TRIM_STR) == 0);

	//str.Trim(IM_STRING_TRIM_LEFT);
	StringTrim(str, STRING_TRIM_LEFT);
	BOOST_CHECK(str.compare(TRIM_STR) == 0);

	str = BOTH_TRIM_STR;
	//str.Trim(IM_STRING_TRIM_BOTH);
	StringTrim(str, STRING_TRIM_BOTH);
	BOOST_CHECK(str.compare(TRIM_STR) == 0);

	str = BLANK_STRING;
	//str.Trim(IM_STRING_TRIM_LEFT);
	StringTrim(str, STRING_TRIM_LEFT);
	BOOST_CHECK(str.compare("") == 0);

	str = BLANK_STRING;
	//str.Trim(IM_STRING_TRIM_RIGHT);
	StringTrim(str, STRING_TRIM_RIGHT);
	BOOST_CHECK(str.compare("") == 0);
}

BOOST_AUTO_TEST_CASE(string_empty)
{
	String_c str;
	
	BOOST_CHECK(StringIsBlank(str));

	str = "   a";	
	BOOST_CHECK(!StringIsBlank(str));

	str = "";
	BOOST_CHECK(StringIsBlank(str));

	str = "bcs";	
	BOOST_CHECK(!StringIsBlank(str));

	str = BLANK_STRING;	
	BOOST_CHECK(StringIsBlank(str));	
}

BOOST_AUTO_TEST_CASE(string_split)
{
	/*
	"bla\nbla" (split by \n)
	"" (splut by \n)
	"\nbla\n" (split by \n)
	"bla\n\nbla" 
	"\n\n"
	"bla"
	"\nbla"
	*/

	String_c str("bla\nblu");

	String_c split;
	size_t it = 0;	
	BOOST_REQUIRE(StringSplitBy(split, str, '\n', it, &it));

	BOOST_REQUIRE(split.compare("bla") == 0);
	BOOST_REQUIRE(it == 4);
	
	BOOST_REQUIRE(StringSplitBy(split, str, '\n', it, &it));

	BOOST_REQUIRE(split.compare("blu")==0);
	BOOST_REQUIRE(it == 7);
	
	BOOST_CHECK(!StringSplitBy(split, str, '\n', it, &it));

	str.clear();
	it = 0;	
	BOOST_CHECK(!StringSplitBy(split, str, '\n', it, &it));

	str = "\nbla\n";
	it = 0;	
	BOOST_REQUIRE(StringSplitBy(split, str, '\n', it, &it));
	BOOST_REQUIRE(it == 5);
	BOOST_REQUIRE(split.compare("bla")==0);
	
	BOOST_CHECK(!StringSplitBy(split, str, '\n', it, &it));

	str = "bla\n\nblu";
	it = 0;	
	BOOST_REQUIRE(StringSplitBy(split, str, '\n', it, &it));
	BOOST_REQUIRE(it == 4);
	BOOST_REQUIRE(split.compare("bla") == 0);
	
	BOOST_REQUIRE(StringSplitBy(split, str, '\n', it, &it));
	BOOST_REQUIRE(it == 8);
	BOOST_REQUIRE(split.compare("blu")==0);
	
	BOOST_CHECK(!StringSplitBy(split, str, '\n', it, &it));

	str = "\n\n";
	it = 0;	
	BOOST_CHECK(!StringSplitBy(split, str, '\n', it, &it));

	str = "bla";
	it = 0;	
	BOOST_REQUIRE(StringSplitBy(split, str, '\n', it, &it));
	BOOST_REQUIRE(it == 3);
	BOOST_REQUIRE(split.compare("bla")==0);
	
	BOOST_CHECK(!StringSplitBy(split, str, '\n', it, &it));

	str = "\nbla";
	it = 0;	
	BOOST_REQUIRE(StringSplitBy(split, str, '\n', it, &it));
	BOOST_REQUIRE(it == 4);
	BOOST_REQUIRE(split.compare("bla") == 0);
	
	BOOST_CHECK(!StringSplitBy(split, str, '\n', it, &it));

	str = "\nbla";
	it = 1024*1024;	
	BOOST_CHECK(!StringSplitBy(split, str, '\n', it, &it));
}

BOOST_AUTO_TEST_CASE(string_replace_all)
{
	String_c str("teste\\bla");
	String_c tmp;

	//replace on dest
	StringReplaceAll(tmp, str, '\\', '/');
	BOOST_REQUIRE(tmp.compare("teste/bla") == 0);
	BOOST_REQUIRE(str.compare("teste\\bla") == 0);

	//replace in place
	StringReplaceAll(str, 'e', '3');
	BOOST_REQUIRE(str.compare("t3st3\\bla") == 0);

	//test no replace, but string should be copied
	StringReplaceAll(tmp, str, 'e', '/');
	BOOST_REQUIRE(tmp.compare("t3st3\\bla") == 0);
}

BOOST_AUTO_TEST_CASE(string_to_boolean)
{
	String_c str("0");

	BOOST_CHECK(StringToBoolean(str) == false);

	str = "1";
	BOOST_CHECK(StringToBoolean(str) == true);

	str = "true";
	BOOST_CHECK(StringToBoolean(str) == true);

	str = "false";
	BOOST_CHECK(StringToBoolean(str) == false);
}
