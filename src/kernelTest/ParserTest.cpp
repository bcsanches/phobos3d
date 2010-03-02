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

#include "PH_Parser.h"

#include <sstream>
#include <iostream>

using namespace Phobos;

using namespace std;

BOOST_AUTO_TEST_CASE(parser_test)
{
	stringstream str;

	str << "123 abc \"string token\"";
	str << endl << "new line" << endl;
	str << "{}()//comment"<< endl;
	str << "end" << endl;

	Parser_c parser;
	parser.SetStream(&str);

	String_c token;
	BOOST_REQUIRE(parser.GetToken(&token)==TOKEN_NUMBER);
	BOOST_REQUIRE(token.compare("123") == 0);

	BOOST_REQUIRE(parser.GetToken(&token)==TOKEN_ID);
	BOOST_REQUIRE(token.compare("abc") == 0);

	BOOST_REQUIRE(parser.GetToken(&token)==TOKEN_STRING);
	BOOST_REQUIRE(token.compare("string token") == 0);
	
	cout<<parser.GetTokenTypeName(parser.GetToken(0))<<endl;

}