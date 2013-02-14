/*
Phobos3d
March 2012
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

#include "Render.h"

#include <PH_Console.h>
#include <PH_Core.h>

PH_DEFINE_DEFAULT_SINGLETON(Render);

Render::Render():
	Module("Render"),
	m_varRScreenX("dvRScreenX", "800"),
	m_varRScreenY("dvRScreenY", "600"),
	m_varRVSync("dvRVSync", "1"),
	m_varRFullScreen("dvRFullScreen", "0"),
	m_cmdScreenshot("screenshot")
{
	using namespace Phobos;

	LogMessage("[Render] Initializing");

	LogMessage("[Render] Initialized.");
}

void Render::OnBoot(void)
{
	using namespace Phobos;

	LogMessage("[Render::OnBoot] Starting");

	m_ipWindow = System::Window::Create("RenderWindow");

	UIntSize_t size(m_varRScreenX.GetInt(), m_varRScreenY.GetInt());
	
	bool fullScreen = m_varRFullScreen.GetBoolean();
	bool vsync = m_varRVSync.GetBoolean();

	LogMessage("[Render::OnBoot] Opening render window");
	m_ipWindow->Open("Phobos Engine", size);

	LogMessage("[Render::OnBoot] Ready.");
	Core::GetInstance().OnEvent(CoreEvents::RENDER_READY);
}

void Render::OnPrepareToBoot()
{
	Phobos::Console &console = Phobos::Console::GetInstance();
	
	console.AddContextCommand(m_cmdScreenshot);
	
	console.AddContextVariable(m_varRScreenX);
	console.AddContextVariable(m_varRScreenY);
	console.AddContextVariable(m_varRFullScreen);
	console.AddContextVariable(m_varRVSync);
}

void Render::CmdScreenshot(const Phobos::Shell::StringVector_t &container, Phobos::Shell::Context &)
{
	//empty
}
