/*
Phobos 3d
March 2010
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

#include <Phobos/Engine/Console.h>
#include <Phobos/Engine/Core.h>
#include <Phobos/Engine/Module.h>
#include <Phobos/Exception.h>
#include <Phobos/Log.h>
#include <Phobos/Memory.h>
#include <Phobos/ObjectManager.h>

#include <Phobos/System/EventManager.h>

using namespace Phobos;
using namespace Phobos::Engine;

enum Sequence_e
{
	NONE,
	TEST_MODULE,
	TEST_MODULE_2
};

static Sequence_e eSequenceMarkerFixedUpdate_gl = NONE;
static Sequence_e eSequenceMarkerUpdate_gl = NONE;

class TestModule: public Engine::Module
{
	public:		
		TestModule():
			Module("test"),
			m_iFixedUpdateCount(0),
			m_iUpdateCount(0),
			m_iInitCount(0),
			m_iFinalizeCount(0)
		{
			++iCount_g;
		}

		~TestModule()
		{
			--iCount_g;
		}

		virtual void OnUpdate() override
		{
			++m_iUpdateCount;
			eSequenceMarkerUpdate_gl= TEST_MODULE;
		}

		virtual void OnFixedUpdate() override
		{
			++m_iFixedUpdateCount;
			eSequenceMarkerFixedUpdate_gl= TEST_MODULE;
		}		

		virtual void OnInit() override
		{
			++m_iInitCount;
		}

		virtual void OnFinalize() override
		{
			++m_iFinalizeCount;
		}

	public:
		static int iCount_g;

		int m_iFixedUpdateCount;
		int m_iUpdateCount;
		int m_iInitCount;
		int m_iFinalizeCount;
};

typedef std::unique_ptr<TestModule> TestModulePtr_t;

int TestModule::iCount_g = 0;

PH_DECLARE_NODE_PTR( TestModule2);

class TestModule2: public Engine::Module
{
	public:
		static TestModule2Ptr_t CreateInstance()
		{
			return TestModule2Ptr_t(PH_NEW TestModule2());
		}
		
		~TestModule2()
		{
		}

		virtual void OnUpdate() override
		{
			eSequenceMarkerUpdate_gl= TEST_MODULE_2;
		}

		virtual void OnFixedUpdate() override
		{
			eSequenceMarkerFixedUpdate_gl= TEST_MODULE_2;
		}		

	protected:
		TestModule2():
			Module("test2")
		{
		}
};

static std::unique_ptr<TestModule> CreateAndRegisterTestModule()
{
	Core &core = Core::GetInstance();

	std::unique_ptr<TestModule> ptr(PH_NEW TestModule());	
	BOOST_REQUIRE(TestModule::iCount_g == 1);

	core.AddModule(*ptr);

	return ptr;
}

class TestConsole: public Console
{
	public:
		static Console &CreateInstance(void)
		{
			Console::UpdateInstance(ConsolePtr_t(PH_NEW TestConsole()));		

			return Console::GetInstance();
		}

	protected:
		TestConsole():
			 Console("Console")
		{
		}

		virtual void OnToggleConsole() override
		{
		}

		virtual void OnEditBoxChanged() override
		{
		}

		virtual void OnTextListChanged() override
		{
		}
};

struct CoreInstance_s
{
	CoreInstance_s()
	{
		char * const argv[] = { "myexe.exe", "set dvExternal boo" };

		auto &console = TestConsole::CreateInstance();

		Phobos::LogChangeFile("engineCoretest.log");		
		Core::CreateInstance(console, "autoexec.cfg", 2, argv);
		
	}

	~CoreInstance_s()
	{
		Console::ReleaseInstance();
		Core::ReleaseInstance();
		
		ObjectManager::Clear();		
	}
};

BOOST_AUTO_TEST_CASE(core_basic)
{
	CoreInstance_s instance;

	Core &core = Core::GetInstance();	

	{
		TestModule module;

		BOOST_REQUIRE_THROW(core.AddModule(module, ModulePriorities::BOOT_MODULE), InvalidParameterException);

	}

	Shell::Variable varExternal("dvExternal", "");
	Console::GetInstance().AddContextVariable(varExternal);

	TestModule *testModule;
	{
		TestModulePtr_t tmp = CreateAndRegisterTestModule();
		testModule = tmp.get();

		BOOST_REQUIRE(TestModule::iCount_g == 1);

#if 0

		core.FixedUpdate(1);
		BOOST_CHECK(testModule->m_iFixedUpdateCount == 1);
		BOOST_CHECK(testModule->m_iUpdateCount == 0);
		BOOST_CHECK(core.GetSimInfo().m_stTimers[Core::TimerTypes::SYSTEM].m_fpFrameTime ==1 );
		BOOST_CHECK(core.GetSimInfo().m_stTimers[Core::TimerTypes::SYSTEM].m_fpTotalTicks == 1);
		BOOST_CHECK(core.GetSimInfo().m_stTimers[Core::TimerTypes::SYSTEM].m_uFrameCount == 1);

		core.FixedUpdate(1);
		BOOST_CHECK(testModule->m_iFixedUpdateCount == 2);
		BOOST_CHECK(testModule->m_iUpdateCount == 0);
		BOOST_CHECK(core.GetSimInfo().m_stTimers[Core::TimerTypes::SYSTEM].m_fpFrameTime == 1);
		BOOST_CHECK(core.GetSimInfo().m_stTimers[Core::TimerTypes::SYSTEM].m_fpTotalTicks == 2);
		BOOST_CHECK(core.GetSimInfo().m_stTimers[Core::TimerTypes::SYSTEM].m_uFrameCount == 2);

		core.Update(1, 0.2f);
		BOOST_CHECK(testModule->m_iFixedUpdateCount == 2);
		BOOST_CHECK(testModule->m_iUpdateCount == 1);
		BOOST_CHECK(core.GetSimInfo().m_stTimers[Core::TimerTypes::SYSTEM].m_fpFrameTime == 1);
		BOOST_CHECK(core.GetSimInfo().m_stTimers[Core::TimerTypes::SYSTEM].m_fpTotalTicks == 2);
		BOOST_CHECK(core.GetSimInfo().m_stTimers[Core::TimerTypes::SYSTEM].m_uFrameCount == 2);
		BOOST_CHECK(core.GetSimInfo().m_stTimers[Core::TimerTypes::SYSTEM].m_fpTotalRenderFrameTime == 1);
		BOOST_CHECK(core.GetSimInfo().m_stTimers[Core::TimerTypes::SYSTEM].m_fpDelta == 0.2f);

		//Fire PrepareToBootEvent
		core.FixedUpdate(1);
		BOOST_CHECK(testModule->m_iPrepareToBootCount == 0);

		core.Update(1, 0.2f);
		BOOST_CHECK(testModule->m_iFixedUpdateCount == 3);
		BOOST_CHECK(testModule->m_iUpdateCount == 2);
		BOOST_CHECK(core.GetSimInfo().m_stTimers[Core::TimerTypes::SYSTEM].m_fpFrameTime == 1);
		BOOST_CHECK(core.GetSimInfo().m_stTimers[Core::TimerTypes::SYSTEM].m_fpTotalTicks == 3);
		BOOST_CHECK(core.GetSimInfo().m_stTimers[Core::TimerTypes::SYSTEM].m_uFrameCount == 3);
		BOOST_CHECK(core.GetSimInfo().m_stTimers[Core::TimerTypes::SYSTEM].m_fpTotalRenderFrameTime == 2);
		BOOST_CHECK(core.GetSimInfo().m_stTimers[Core::TimerTypes::SYSTEM].m_fpDelta == 0.2f);
		BOOST_CHECK(testModule->m_iPrepareToBootCount == 1);

		LogMessage("Test reminder: ignore autoexec.cfg not found message");

		//Force boot event fire
		core.FixedUpdate(1);
		core.FixedUpdate(1);
		core.FixedUpdate(1);

		//Force boot dispatch
		core.FixedUpdate(1);
		BOOST_CHECK(testModule->m_iBootCount);

		//Check if the external command line was used
		BOOST_CHECK(varExternal.GetValue().compare("boo") == 0);

		core.AddModuleToDestroyList(*testModule);
		BOOST_REQUIRE(TestModule::iCount_g == 1);

		core.FixedUpdate(1);
		BOOST_REQUIRE(TestModule::iCount_g == 1);
		BOOST_CHECK(testModule->m_iFixedUpdateCount == 7);
		BOOST_CHECK(testModule->m_iUpdateCount == 2);

		BOOST_REQUIRE_THROW(core.AddModuleToDestroyList(*testModule), ObjectNotFoundException);

#endif
	}

#if 0
	BOOST_REQUIRE(TestModule::iCount_g == 0);

	BOOST_CHECK(core.GetSimInfo().m_stTimers[Core::TimerTypes::GAME].m_fpFrameTime == 1);
	BOOST_CHECK(core.GetSimInfo().m_stTimers[Core::TimerTypes::GAME].m_fpTotalTicks == 8);
	BOOST_CHECK(core.GetSimInfo().m_stTimers[Core::TimerTypes::GAME].m_uFrameCount == 8);
	BOOST_CHECK(core.GetSimInfo().m_stTimers[Core::TimerTypes::GAME].m_fpTotalRenderFrameTime == 2);
	BOOST_CHECK(core.GetSimInfo().m_stTimers[Core::TimerTypes::GAME].m_fpDelta == 0.2f);

	core.PauseTimer(Core::TimerTypes::GAME);
	core.FixedUpdate(2);
	core.Update(3, 0.5f);

	BOOST_CHECK(core.GetSimInfo().m_stTimers[Core::TimerTypes::GAME].m_fpFrameTime == 1);
	BOOST_CHECK(core.GetSimInfo().m_stTimers[Core::TimerTypes::GAME].m_fpTotalTicks == 8);
	BOOST_CHECK(core.GetSimInfo().m_stTimers[Core::TimerTypes::GAME].m_uFrameCount == 8);
	BOOST_CHECK(core.GetSimInfo().m_stTimers[Core::TimerTypes::GAME].m_fpTotalRenderFrameTime == 2);
	BOOST_CHECK(core.GetSimInfo().m_stTimers[Core::TimerTypes::GAME].m_fpDelta == 0.2f);

	BOOST_CHECK(core.GetSimInfo().m_stTimers[Core::TimerTypes::SYSTEM].m_fpFrameTime == 2);
	BOOST_CHECK(core.GetSimInfo().m_stTimers[Core::TimerTypes::SYSTEM].m_fpTotalTicks == 10);
	BOOST_CHECK(core.GetSimInfo().m_stTimers[Core::TimerTypes::SYSTEM].m_uFrameCount == 9);
	BOOST_CHECK(core.GetSimInfo().m_stTimers[Core::TimerTypes::SYSTEM].m_fpTotalRenderFrameTime == 5);
	BOOST_CHECK(core.GetSimInfo().m_stTimers[Core::TimerTypes::SYSTEM].m_fpDelta == 0.5f);
#endif
}

BOOST_AUTO_TEST_CASE(core_sorting)
{
	CoreInstance_s instance;

	TestModulePtr_t test = CreateAndRegisterTestModule();

	TestModule2Ptr_t ptr = TestModule2::CreateInstance();

	Core &core = Core::GetInstance();
	core.AddModule(*ptr, ModulePriorities::LOWEST);

#if 0
	core.FixedUpdate(1);
	BOOST_CHECK(eSequenceMarkerFixedUpdate_gl == TEST_MODULE_2);

	core.AddModuleToDestroyList(*(ptr.get()));
	core.FixedUpdate(1);
	BOOST_CHECK(eSequenceMarkerFixedUpdate_gl == TEST_MODULE);

	core.AddModule(*ptr, ModulePriorities::HIGHEST);
	core.FixedUpdate(1);
	BOOST_CHECK(eSequenceMarkerFixedUpdate_gl == TEST_MODULE);

	core.AddModuleToDestroyList(*(test.get()));
	core.FixedUpdate(1);
	BOOST_CHECK(eSequenceMarkerFixedUpdate_gl == TEST_MODULE_2);

	core.Update(1, 0);
	BOOST_CHECK(eSequenceMarkerUpdate_gl == TEST_MODULE_2);
#endif
}
