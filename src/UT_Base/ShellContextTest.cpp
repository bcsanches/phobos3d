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
#include <fstream>

#include <boost/test/unit_test.hpp>

#include <Phobos/Shell/Context.h>
#include <Phobos/Shell/Utils.h>
#include <Phobos/Exception.h>

using namespace Phobos;
using namespace Phobos::Shell;

class IfCmdTest
{
	public:
		enum Modes_e
		{
			THEN_MODE,
			ELSE_MODE
		};

		IfCmdTest(Context &context, Modes_e mode):
			cmdThenTest("thenTest"),
			cmdElseTest("elseTest"),
			eMode(mode)
		{
			cmdThenTest.SetProc(PH_CONTEXT_CMD_BIND(&IfCmdTest::ThenTestCmd, this));
			cmdElseTest.SetProc(PH_CONTEXT_CMD_BIND(&IfCmdTest::ElseTestCmd, this));

			context.AddContextCommand(cmdThenTest);
			context.AddContextCommand(cmdElseTest);
		}

	private:
		void ThenTestCmd(const StringVector_t &args, Context &)
		{
			BOOST_REQUIRE(eMode == THEN_MODE);
		}

		void ElseTestCmd(const StringVector_t &args, Context &)
		{
			BOOST_REQUIRE(eMode == ELSE_MODE);
		}

		Command	cmdThenTest;
		Command cmdElseTest;

		Modes_e			eMode;
};

static void CheckArgsCmd(const StringVector_t &args, Context &)
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
	Context context;

	Variable tempVar("temp", "empty");
	context.AddContextVariable(tempVar);

	BOOST_REQUIRE_THROW(context.AddContextVariable(tempVar), ObjectAlreadyExistsException);

	context.Execute("set temp bla");
	BOOST_REQUIRE(tempVar.GetValue().compare("bla") == 0);

	Variable pointerVar("pointer", "temp");
	context.AddContextVariable(pointerVar);

	context.Execute("set $pointer 5");
	BOOST_REQUIRE(tempVar.GetInt() == 5);

	Command cmdCheckArgs("checkArgs", CheckArgsCmd);
	context.AddContextCommand(cmdCheckArgs);

	context.Execute("checkArgs 1 2 3 4 $temp \"my string test\";");

	{
		IfCmdTest ifTest(context, IfCmdTest::THEN_MODE);

		context.Execute("set temp true;if $temp thenTest elseTest");

		//test wihout else block
		context.Execute("if $temp thenTest");
	}

	{
		IfCmdTest ifTest(context, IfCmdTest::ELSE_MODE);

		context.Execute("set temp 0;if $temp thenTest elseTest");
	}	
}


BOOST_AUTO_TEST_CASE(context_file)
{	
	Context context;

	BOOST_REQUIRE_THROW(context.ExecuteFromFile("fileThatShouldNotExits.txt"), FileNotFoundException);

	Variable tempVar("temp", "empty");
	context.AddContextVariable(tempVar);

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
