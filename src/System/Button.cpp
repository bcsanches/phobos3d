/*
Phobos 3d
October 2010
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

#include "Phobos/System/Button.h"

#include <PH_Context.h>
#include <PH_ContextUtils.h>
#include <PH_Error.h>


Phobos::System::Button_c::Button_c(const String_c &up, const String_c &down, const String_c &update, IContext_c *context):
	fpValue(0),
	cmdUp(up),
	cmdDown(down),
	cmdUpdate(update)
{
	cmdUp.SetProc(PH_CONTEXT_CMD_BIND(&Button_c::CmdProc, this));
	cmdDown.SetProc(PH_CONTEXT_CMD_BIND(&Button_c::CmdProc, this));
	cmdUpdate.SetProc(PH_CONTEXT_CMD_BIND(&Button_c::CmdProc, this));

	if(context)
		this->Enable(*context);
}

void Phobos::System::Button_c::Enable(IContext_c &context)
{
	context.AddContextCmd(cmdUp);
	context.AddContextCmd(cmdDown);
	context.AddContextCmd(cmdUpdate);
}

void Phobos::System::Button_c::Disable()
{
	cmdUp.Unlink();
	cmdDown.Unlink();
	cmdUpdate.Unlink();

	fpValue = 0;
}

void Phobos::System::Button_c::CmdProc(const StringVector_t &args, Context_c & )
{
	PH_ASSERT(args.size() >= 4);

	fpValue = StringToFloat(args[3]);
}	
