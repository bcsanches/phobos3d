/*
Phobos 3d
  March 2010

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

#include <fstream>

#include <boost/test/unit_test.hpp>

#include <PH_Context.h>
#include <PH_ContextUtils.h>
#include <PH_Exception.h>

using namespace Phobos;

class IfCmdTest_c
{
	public:
		enum Modes_e
		{
			THEN_MODE,
			ELSE_MODE
		};

		IfCmdTest_c(Context_c &context, Modes_e mode):
			cmdThenTest("thenTest"),
			cmdElseTest("elseTest"),
			eMode(mode)
		{
			cmdThenTest.SetProc(PH_CONTEXT_CMD_BIND(&IfCmdTest_c::ThenTestCmd, this));
			cmdElseTest.SetProc(PH_CONTEXT_CMD_BIND(&IfCmdTest_c::ElseTestCmd, this));

			context.AddContextCmd(cmdThenTest);
			context.AddContextCmd(cmdElseTest);
		}

	private:
		void ThenTestCmd(const StringVector_t &args, Context_c &)
		{
			BOOST_REQUIRE(eMode == THEN_MODE);
		}

		void ElseTestCmd(const StringVector_t &args, Context_c &)
		{
			BOOST_REQUIRE(eMode == ELSE_MODE);
		}

		ContextCmd_c	cmdThenTest;
		ContextCmd_c	cmdElseTest;

		Modes_e			eMode;
};

static void CheckArgsCmd(const StringVector_t &args, Context_c &)
{
	BOOST_REQUIRE(args.size() == 7);

	BOOST_REQUIRE(args[0].compare("checkArgs") == 0);
	BOOST_REQUIRE(args[1].compare("1") == 0);
	BOOST_REQUIRE(args[2].compare("2") == 0);
	BOOST_REQUIRE(args[3].compare("3") == 0);
	BOOST_REQUIRE(args[4].compare("4") == 0);
	BOOST_REQUIRE(args[5].compare("5") == 0);
	BOOST_REQUIRE(args[6].compare("my string test") == 0);
}

BOOST_AUTO_TEST_CASE(context_basic)
{
	Context_c context;

	ContextVar_c tempVar("temp", "empty");
	context.AddContextVar(tempVar);

	BOOST_REQUIRE_THROW(context.AddContextVar(tempVar), ObjectAlreadyExistsException_c);

	context.Execute("set temp bla");
	BOOST_REQUIRE(tempVar.GetValue().compare("bla") == 0);

	ContextVar_c pointerVar("pointer", "temp");
	context.AddContextVar(pointerVar);

	context.Execute("set $pointer 5");
	BOOST_REQUIRE(tempVar.GetInt() == 5);

	ContextCmd_c cmdCheckArgs("checkArgs", CheckArgsCmd);
	context.AddContextCmd(cmdCheckArgs);

	context.Execute("checkArgs 1 2 3 4 $temp \"my string test\";");

	{
		IfCmdTest_c ifTest(context, IfCmdTest_c::THEN_MODE);

		context.Execute("set temp true;if $temp thenTest elseTest");

		//test wihout else block
		context.Execute("if $temp thenTest");
	}

	{
		IfCmdTest_c ifTest(context, IfCmdTest_c::ELSE_MODE);

		context.Execute("set temp 0;if $temp thenTest elseTest");
	}
}


BOOST_AUTO_TEST_CASE(context_file)
{
	Context_c context;

	BOOST_REQUIRE_THROW(context.ExecuteFromFile("fileThatShouldNotExits.txt"), FileNotFoundException_c);

	ContextVar_c tempVar("temp", "empty");
	context.AddContextVar(tempVar);

	{
		using namespace std;
		ofstream outf("context_file.cfg", ios_base::trunc);

		outf << "set temp bla" << endl;
		outf <<"//comment" << endl;

		outf.flush();
		BOOST_REQUIRE(outf.good());
	}

	context.ExecuteFromFile("context_file.cfg");

	BOOST_REQUIRE(tempVar.GetValue().compare("bla") == 0);
}
