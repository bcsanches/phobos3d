/*
Phobos3d
March 2012
Copyright (c) 2005-2012 Bruno Sanches  http://code.google.com/p/phobos3d

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#include "Render.h"

#include <PH_Console.h>
#include <PH_Core.h>
#include <PH_Kernel.h>

PH_DEFINE_DEFAULT_SINGLETON(Render);

Render_c::Render_c():
	CoreModule_c("Render"),
	varRScreenX("dvRScreenX", "800"),
	varRScreenY("dvRScreenY", "600"),
	varRVSync("dvRVSync", "1"),
	varRFullScreen("dvRFullScreen", "0"),
	cmdScreenshot("screenshot")
{
	using namespace Phobos;

	Kernel_c &kernel = Kernel_c::GetInstance();

	kernel.LogMessage("[Render] Initializing");

	kernel.LogMessage("[Render] Initialized.");
}

void Render_c::OnBoot(void)
{
	using namespace Phobos;

	Kernel_c	&kernel(Kernel_c::GetInstance());
	kernel.LogMessage("[Render_c::OnBoot] Starting");

	ipWindow = Window_c::Create("RenderWindow");

	Rect_s<UInt_t> r;

	r.tOrigin[0] = 0;
	r.tOrigin[1] = 0;
	r.tWidth = varRScreenX.GetInt();
	r.tHeight = varRScreenY.GetInt();
	bool fullScreen = varRFullScreen.GetBoolean();
	bool vsync = varRVSync.GetBoolean();

	kernel.LogMessage("[Render_c::OnBoot] Opening render window");
	ipWindow->Open("Phobos Engine", r);

	kernel.LogMessage("[Render_c::OnBoot] Ready.");
	Core_c::GetInstance()->OnEvent(CORE_EVENT_RENDER_READY);
}

void Render_c::OnPrepareToBoot()
{
	Phobos::ConsolePtr_t console = Phobos::Console_c::GetInstance();
	
	console->AddContextCmd(cmdScreenshot);
	
	console->AddContextVar(varRScreenX);
	console->AddContextVar(varRScreenY);
	console->AddContextVar(varRFullScreen);
	console->AddContextVar(varRVSync);
}

void Render_c::CmdScreenshot(const Phobos::StringVector_t &container, Phobos::Context_c &)
{
	//empty
}
