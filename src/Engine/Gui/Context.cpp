/*
Phobos 3d
September 2012
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


#include "Phobos/Engine/Gui/Context.h"

#include <Phobos/System/InputDevice.h>
#include <Phobos/System/InputEvent.h>

Phobos::Engine::Gui::ContextPtr_t Phobos::Engine::Gui::Context::Create(const String_t &name, size_t screenWidth, size_t screenHeight)
{
	return std::make_shared<Context>(name, screenWidth, screenHeight);
}

Phobos::Engine::Gui::Context::Context(const String_t &name, size_t screenWidth, size_t screenHeight):
	Node(name),
	m_pclContext(NULL)
{	
	m_pclContext = Rocket::Core::CreateContext(name.c_str(), Rocket::Core::Vector2i(screenWidth, screenHeight));
}

Phobos::Engine::Gui::Context::~Context()
{
	m_pclContext->RemoveReference();
}

void Phobos::Engine::Gui::Context::OnInputEvent(const System::InputEvent_s &event)
{
	if(event.m_pclDevice->GetDeviceType() == System::INPUT_DEVICE_MOUSE)
	{
		if(event.m_eType == System::INPUT_EVENT_THUMB)
		{
			m_pclContext->ProcessMouseMove(static_cast<int>(event.m_stThumb.m_fpAxis[0]), static_cast<int>(event.m_stThumb.m_fpAxis[1]), 0);
		}
		else if(event.m_eType == System::INPUT_EVENT_BUTTON)
		{
			if(event.m_stButton.m_fpPression > 0)
				m_pclContext->ProcessMouseButtonDown(event.m_stButton.m_uId-1, 0);
			else
				m_pclContext->ProcessMouseButtonUp(event.m_stButton.m_uId-1, 0);
		}	
	}
}

void Phobos::Engine::Gui::Context::ResetMousePosition(short x, short y)
{
	m_pclContext->ProcessMouseMove(x, y, 0);
}

void Phobos::Engine::Gui::Context::Update()
{
	m_pclContext->Update();	
}

void Phobos::Engine::Gui::Context::Render()
{
	m_pclContext->Render();
}

Rocket::Core::ElementDocument *Phobos::Engine::Gui::Context::LoadMouseCursor(const Char_t *path)
{
	return m_pclContext->LoadMouseCursor(path);
}

Rocket::Core::ElementDocument *Phobos::Engine::Gui::Context::LoadDocument(const Char_t *path)
{
	return m_pclContext->LoadDocument(path);
}


