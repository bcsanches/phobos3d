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


#include "Gui/PH_Context.h"

#include <boost/make_shared.hpp>

#include <Phobos/System/InputDevice.h>
#include <Phobos/System/InputEvent.h>

Phobos::Gui::ContextPtr_t Phobos::Gui::Context_c::Create(const String_t &name, size_t screenWidth, size_t screenHeight)
{
	return boost::make_shared<Context_c>(name, screenWidth, screenHeight);
}

Phobos::Gui::Context_c::Context_c(const String_t &name, size_t screenWidth, size_t screenHeight):
	Node_c(name),
	pclContext(NULL)
{	
	pclContext = Rocket::Core::CreateContext(name.c_str(), Rocket::Core::Vector2i(screenWidth, screenHeight));
}

Phobos::Gui::Context_c::~Context_c()
{
	pclContext->RemoveReference();
}

void Phobos::Gui::Context_c::InputEvent(const System::InputEvent_s &event)
{
	if(event.pclDevice->GetDeviceType() == System::INPUT_DEVICE_MOUSE)
	{
		if(event.eType == System::INPUT_EVENT_THUMB)
		{
			pclContext->ProcessMouseMove(static_cast<int>(event.stThumb.fpAxis[0]), static_cast<int>(event.stThumb.fpAxis[1]), 0);
		}
		else if(event.eType == System::INPUT_EVENT_BUTTON)
		{
			if(event.stButton.fpPression > 0)
				pclContext->ProcessMouseButtonDown(event.stButton.uId-1, 0);
			else
				pclContext->ProcessMouseButtonUp(event.stButton.uId-1, 0);
		}	
	}
}

void Phobos::Gui::Context_c::ResetMousePosition(short x, short y)
{
	pclContext->ProcessMouseMove(x, y, 0);
}

void Phobos::Gui::Context_c::Update()
{
	pclContext->Update();	
}

void Phobos::Gui::Context_c::Render()
{
	pclContext->Render();
}

Rocket::Core::ElementDocument *Phobos::Gui::Context_c::LoadMouseCursor(const Char_t *path)
{
	return pclContext->LoadMouseCursor(path);
}

Rocket::Core::ElementDocument *Phobos::Gui::Context_c::LoadDocument(const Char_t *path)
{
	return pclContext->LoadDocument(path);
}


