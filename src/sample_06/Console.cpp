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


#include "Console.h"

Console &Console::CreateInstance(void)
{		
	Phobos::Engine::Console::UpdateInstance(std::make_shared<Console>());

	return static_cast<Console&>(Phobos::Engine::Console::GetInstance());
}

Console::Console(void):
	Phobos::Engine::Console("Console"),		
	m_fEditBoxChanged(true),
	m_fTextBufferChanged(true)
{				
	//empty
}

Console::~Console(void)
{
	//empty
}	

void Console::OnEditBoxChanged()
{
	m_fEditBoxChanged = true;
}

void Console::OnTextListChanged()
{
	m_fTextBufferChanged = true;
}

void Console::OnUpdate(void)
{
	m_fTextBufferChanged = m_fEditBoxChanged = false;		
}

void Console::OnRenderReady(void)
{
	//empty
}	

void Console::OnToggleConsole()
{
	//empty
}	
