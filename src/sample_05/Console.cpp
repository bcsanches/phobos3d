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

ConsolePtr_t Console_c::CreateInstance(void)
{		
	Phobos::Console_c::UpdateInstance(new Console_c());		

	return boost::static_pointer_cast<Console_c>(Phobos::Console_c::GetInstance());
}

Console_c::Console_c(void):
	Phobos::Console_c("Console"),		
	fEditBoxChanged(true),
	fTextBufferChanged(true)
{				
	//empty
}

Console_c::~Console_c(void)
{
	//empty
}	

void Console_c::OnEditBoxChanged()
{
	fEditBoxChanged = true;
}

void Console_c::OnTextListChanged()
{
	fTextBufferChanged = true;
}

void Console_c::OnUpdate(void)
{
	fTextBufferChanged = fEditBoxChanged = false;		
}

void Console_c::OnRenderReady(void)
{
	//empty
}	

void Console_c::OnToggleConsole()
{
	//empty
}	
