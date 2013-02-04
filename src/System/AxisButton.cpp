/*
Phobos 3d
October 2010
Copyright (c) 2005-2011 Bruno Sanches  http://code.google.com/p/phobos3d

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#include "Phobos/System/AxisButton.h"

Phobos::System::AxisButton::AxisButton(const String_t &upA, const String_t &downA, const String_t &updateA, const String_t &upB, const String_t &downB, const String_t &updateB, Shell::IContext *context):
	m_clButtonA(upA, downA, updateA, context),
	m_clButtonB(upB, downB, updateB, context)
{
	//empty
}

void Phobos::System::AxisButton::Enable(Shell::IContext &context)
{
	m_clButtonA.Enable(context);
	m_clButtonB.Enable(context);
}

void Phobos::System::AxisButton::Disable()
{
	m_clButtonA.Disable();
	m_clButtonB.Disable();
}

