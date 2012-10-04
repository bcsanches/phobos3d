/*
Phobos 3d
September 2012
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


#include "Gui/PH_Context.h"

#include <boost/make_shared.hpp>

#include <PH_Render.h>

Phobos::Gui::ContextPtr_t Phobos::Gui::Context_c::Create(const String_c &name)
{
	return boost::make_shared<Context_c>(name);
}

Phobos::Gui::Context_c::Context_c(const String_c &name):
	Node_c(name),
	pclContext(NULL)
{
	Phobos::Render_c &render = Render_c::GetInstance();	

	pclContext = Rocket::Core::CreateContext(name.c_str(), Rocket::Core::Vector2i(render.GetScreenWidth(), render.GetScreenHeight()));
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
