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

Phobos::System::AxisButton_c::AxisButton_c(const String_c &upA, const String_c &downA, const String_c &updateA, const String_c &upB, const String_c &downB, const String_c &updateB, IContext_c *context):
	clButtonA(upA, downA, updateA, context),
	clButtonB(upB, downB, updateB, context)
{
	//empty
}

void Phobos::System::AxisButton_c::Enable(IContext_c &context)
{
	clButtonA.Enable(context);
	clButtonB.Enable(context);
}

void Phobos::System::AxisButton_c::Disable()
{
	clButtonA.Disable();
	clButtonB.Disable();
}
