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

#include <Phobos/Engine/Console.h>

#include <SDL.h>

#include <cmath>

PH_DEFINE_DEFAULT_SINGLETON2(Render, Phobos::Engine::Console &);

Render::Render(Phobos::Engine::Console &console) :
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

	console.AddContextCommand(m_cmdScreenshot);

	console.AddContextVariable(m_varRScreenX);
	console.AddContextVariable(m_varRScreenY);
	console.AddContextVariable(m_varRFullScreen);
	console.AddContextVariable(m_varRVSync);
}

void Render::OnInit()
{
	using namespace Phobos;

	LogMessage("[Render::OnInit] Starting");

	m_ipWindow = System::Window::Create("RenderWindow");

	UIntSize_t size(m_varRScreenX.GetInt(), m_varRScreenY.GetInt());
	
	bool fullScreen = m_varRFullScreen.GetBoolean();
	bool vsync = m_varRVSync.GetBoolean();

	LogMessage("[Render::OnInit] Opening render window");
	m_ipWindow->Open("Phobos Engine", size, 0);

	LogMessage("[Render::OnInit] Ready.");
}

void Render::Clear()
{
	//clear it
	SDL_FillRect(SDL_GetVideoSurface(), nullptr, 0x0);
}

void Render::Draw(const Sprite &object)
{
	auto surface = SDL_GetVideoSurface();	

	SDL_Rect rect;

	rect.x = static_cast<Sint16>(std::lround(object.m_fpX));
	rect.y = static_cast<Sint16>(std::lround(object.m_fpY));
	rect.w = object.m_iW;
	rect.h = object.m_iH;

	SDL_FillRect(surface, &rect, object.m_u32Color);
}

void Render::OnUpdate()
{
	auto surface = SDL_GetVideoSurface();		

	SDL_UpdateRect(surface, 0, 0, surface->w, surface->h);	
}

int Render::GetWindowWidth()
{
	return m_ipWindow->GetWidth();
}
int Render::GetWindowHeight()
{
	return m_ipWindow->GetHeight();
}

void Render::CmdScreenshot(const Phobos::Shell::StringVector_t &container, Phobos::Shell::Context &)
{
	//empty
}
