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

#include <boost/test/unit_test.hpp>

#include <PH_Console.h>
#include <PH_Core.h>
#include <PH_CoreModule.h>
#include <PH_Exception.h>
#include <PH_Kernel.h>

using namespace Phobos;

enum Sequence_e
{
	NONE,
	TEST_MODULE,
	TEST_MODULE_2
};

static Sequence_e eSequenceMarkerFixedUpdate_gl = NONE;
static Sequence_e eSequenceMarkerUpdate_gl = NONE;

class TestModule_c;

typedef boost::intrusive_ptr<TestModule_c> TestModulePtr_t;

class TestModule_c: public CoreModule_c
{
	public:
		static TestModulePtr_t CreateInstance()
		{
			return TestModulePtr_t(new TestModule_c());
		}

		virtual void OnUpdate() 
		{
			++iUpdateCount;
			eSequenceMarkerUpdate_gl= TEST_MODULE;
		}

		virtual void OnFixedUpdate() 
		{
			++iFixedUpdateCount;
			eSequenceMarkerFixedUpdate_gl= TEST_MODULE;
		}

		virtual void OnPrepareToBoot()
		{
			++iPrepareToBootCount;
		}

		virtual void OnBoot() 
		{
			++iBootCount;
		}

		virtual void OnFinalize() 
		{
			++iFinalizeCount;
		}

	protected:
		TestModule_c(): 
			CoreModule_c("test"),
			iFixedUpdateCount(0),
			iUpdateCount(0),
			iBootCount(0),
			iFinalizeCount(0),
			iPrepareToBootCount(0)
		{
			++iCount;
		}

		~TestModule_c()
		{
			--iCount;
		}		

	public:
		static int iCount;

		int iFixedUpdateCount;
		int iUpdateCount;
		int iBootCount;
		int iPrepareToBootCount;
		int iFinalizeCount;
};

int TestModule_c::iCount = 0;

class TestModule2_c;

typedef boost::intrusive_ptr<TestModule2_c> TestModule2Ptr_t;

class TestModule2_c: public CoreModule_c
{
	public:
		static TestModule2Ptr_t CreateInstance()
		{
			return TestModule2Ptr_t(new TestModule2_c());
		}

		virtual void OnUpdate() 
		{			
			eSequenceMarkerUpdate_gl= TEST_MODULE_2;
		}

		virtual void OnFixedUpdate() 
		{			
			eSequenceMarkerFixedUpdate_gl= TEST_MODULE_2;
		}

		virtual void OnBoot() 
		{
			
		}

		virtual void OnFinalize() 
		{
			
		}

	protected:
		TestModule2_c(): 
			CoreModule_c("test2")
		{			
		}

		~TestModule2_c()
		{			
		}					
};

static TestModulePtr_t CreateAndRegisterTestModule()
{
	CorePtr_t core = Core_c::GetInstance();
	
	TestModulePtr_t ptr = TestModule_c::CreateInstance();
	BOOST_REQUIRE(TestModule_c::iCount == 1);	

	core->AddModule(ptr);

	return ptr;
}

struct CoreInstance_s
{
	CoreInstance_s()
	{
		Kernel_c::CreateInstance("enginecoretest.log");
		Core_c::CreateInstance();
		Console_c::CreateInstance();
	}

	~CoreInstance_s()
	{
		Console_c::ReleaseInstance();
		Core_c::ReleaseInstance();

		Kernel_c::ReleaseInstance();
	}
};

BOOST_AUTO_TEST_CASE(core_basic)
{
	CoreInstance_s instance;

	CorePtr_t core = Core_c::GetInstance();	

	BOOST_CHECK_THROW(core->PauseTimer(CORE_SYS_TIMER), InvalidParameterException_c);

	{
		TestModulePtr_t ptr = TestModule_c::CreateInstance();

		BOOST_REQUIRE_THROW(core->AddModule(ptr, BOOT_MODULE_PRIORITY), InvalidParameterException_c);
	}

	core->LaunchBootModule();

	TestModule_c *testModule;
	{
		TestModulePtr_t tmp = CreateAndRegisterTestModule();
		testModule = tmp.get();
	
		BOOST_REQUIRE(TestModule_c::iCount == 1);

		core->FixedUpdate(1);
		BOOST_CHECK(testModule->iFixedUpdateCount == 1);
		BOOST_CHECK(testModule->iUpdateCount == 0);
		BOOST_CHECK(core->GetSimInfo().stTimers[CORE_SYS_TIMER].fpFrameTime ==1 );
		BOOST_CHECK(core->GetSimInfo().stTimers[CORE_SYS_TIMER].fpTotalTicks == 1);
		BOOST_CHECK(core->GetSimInfo().stTimers[CORE_SYS_TIMER].uFrameCount == 1);

		core->FixedUpdate(1);
		BOOST_CHECK(testModule->iFixedUpdateCount == 2);
		BOOST_CHECK(testModule->iUpdateCount == 0);
		BOOST_CHECK(core->GetSimInfo().stTimers[CORE_SYS_TIMER].fpFrameTime == 1);
		BOOST_CHECK(core->GetSimInfo().stTimers[CORE_SYS_TIMER].fpTotalTicks == 2);
		BOOST_CHECK(core->GetSimInfo().stTimers[CORE_SYS_TIMER].uFrameCount == 2);

		core->Update(1, 0.2f);
		BOOST_CHECK(testModule->iFixedUpdateCount == 2);
		BOOST_CHECK(testModule->iUpdateCount == 1);
		BOOST_CHECK(core->GetSimInfo().stTimers[CORE_SYS_TIMER].fpFrameTime == 1);
		BOOST_CHECK(core->GetSimInfo().stTimers[CORE_SYS_TIMER].fpTotalTicks == 2);
		BOOST_CHECK(core->GetSimInfo().stTimers[CORE_SYS_TIMER].uFrameCount == 2);
		BOOST_CHECK(core->GetSimInfo().stTimers[CORE_SYS_TIMER].fpTotalRenderFrameTime == 1);
		BOOST_CHECK(core->GetSimInfo().stTimers[CORE_SYS_TIMER].fpDelta == 0.2f);

		//Fire PrepareToBootEvent
		core->FixedUpdate(1);
		BOOST_CHECK(testModule->iPrepareToBootCount == 0);		

		core->Update(1, 0.2f);
		BOOST_CHECK(testModule->iFixedUpdateCount == 3);
		BOOST_CHECK(testModule->iUpdateCount == 2);
		BOOST_CHECK(core->GetSimInfo().stTimers[CORE_SYS_TIMER].fpFrameTime == 1);
		BOOST_CHECK(core->GetSimInfo().stTimers[CORE_SYS_TIMER].fpTotalTicks == 3);
		BOOST_CHECK(core->GetSimInfo().stTimers[CORE_SYS_TIMER].uFrameCount == 3);
		BOOST_CHECK(core->GetSimInfo().stTimers[CORE_SYS_TIMER].fpTotalRenderFrameTime == 2);
		BOOST_CHECK(core->GetSimInfo().stTimers[CORE_SYS_TIMER].fpDelta == 0.2f);
		BOOST_CHECK(testModule->iPrepareToBootCount == 1);

		//Force boot event fire
		core->FixedUpdate(1);
		core->FixedUpdate(1);
		core->FixedUpdate(1);

		//Force boot dispatch
		core->FixedUpdate(1);		
		BOOST_CHECK(testModule->iBootCount);

		core->AddModuleToDestroyList(*testModule);
		BOOST_REQUIRE(TestModule_c::iCount == 1);

		core->FixedUpdate(1);
		BOOST_REQUIRE(TestModule_c::iCount == 1);	
		BOOST_CHECK(testModule->iFixedUpdateCount == 7);
		BOOST_CHECK(testModule->iUpdateCount == 2);		

		BOOST_REQUIRE_THROW(core->AddModuleToDestroyList(*testModule), ObjectNotFoundException_c);
	}

	BOOST_REQUIRE(TestModule_c::iCount == 0);	

	BOOST_CHECK(core->GetSimInfo().stTimers[CORE_GAME_TIMER].fpFrameTime == 1);
	BOOST_CHECK(core->GetSimInfo().stTimers[CORE_GAME_TIMER].fpTotalTicks == 8);
	BOOST_CHECK(core->GetSimInfo().stTimers[CORE_GAME_TIMER].uFrameCount == 8);
	BOOST_CHECK(core->GetSimInfo().stTimers[CORE_GAME_TIMER].fpTotalRenderFrameTime == 2);
	BOOST_CHECK(core->GetSimInfo().stTimers[CORE_GAME_TIMER].fpDelta == 0.2f);	

	core->PauseTimer(CORE_GAME_TIMER);
	core->FixedUpdate(2);
	core->Update(3, 0.5f);

	BOOST_CHECK(core->GetSimInfo().stTimers[CORE_GAME_TIMER].fpFrameTime == 1);
	BOOST_CHECK(core->GetSimInfo().stTimers[CORE_GAME_TIMER].fpTotalTicks == 8);
	BOOST_CHECK(core->GetSimInfo().stTimers[CORE_GAME_TIMER].uFrameCount == 8);
	BOOST_CHECK(core->GetSimInfo().stTimers[CORE_GAME_TIMER].fpTotalRenderFrameTime == 2);
	BOOST_CHECK(core->GetSimInfo().stTimers[CORE_GAME_TIMER].fpDelta == 0.2f);	

	BOOST_CHECK(core->GetSimInfo().stTimers[CORE_SYS_TIMER].fpFrameTime == 2);
	BOOST_CHECK(core->GetSimInfo().stTimers[CORE_SYS_TIMER].fpTotalTicks == 10);
	BOOST_CHECK(core->GetSimInfo().stTimers[CORE_SYS_TIMER].uFrameCount == 9);
	BOOST_CHECK(core->GetSimInfo().stTimers[CORE_SYS_TIMER].fpTotalRenderFrameTime == 5);
	BOOST_CHECK(core->GetSimInfo().stTimers[CORE_SYS_TIMER].fpDelta == 0.5f);
}

BOOST_AUTO_TEST_CASE(core_sorting)
{
	CoreInstance_s instance;

	TestModulePtr_t test = CreateAndRegisterTestModule();
	
	TestModule2Ptr_t ptr = TestModule2_c::CreateInstance();

	CorePtr_t core = Core_c::GetInstance();
	core->AddModule(ptr, LOWEST_PRIORITY);

	core->FixedUpdate(1);
	BOOST_CHECK(eSequenceMarkerFixedUpdate_gl == TEST_MODULE_2);

	core->AddModuleToDestroyList(*(ptr.get()));
	core->FixedUpdate(1);
	BOOST_CHECK(eSequenceMarkerFixedUpdate_gl == TEST_MODULE);

	core->AddModule(ptr, HIGHEST_PRIORITY);
	core->FixedUpdate(1);
	BOOST_CHECK(eSequenceMarkerFixedUpdate_gl == TEST_MODULE);

	core->AddModuleToDestroyList(*(test.get()));
	core->FixedUpdate(1);
	BOOST_CHECK(eSequenceMarkerFixedUpdate_gl == TEST_MODULE_2);

	core->Update(1, 0);
	BOOST_CHECK(eSequenceMarkerUpdate_gl == TEST_MODULE_2);	
}