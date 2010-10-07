/*
Phobos 3d
  October 2010

  Copyright (C) 2005-2010 Bruno Crivelari Sanches

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Bruno Crivelari Sanches bcsanches@gmail.com
*/

#include "PH_Button.h"

#include <PH_Context.h>
#include <PH_ContextUtils.h>
#include <PH_Error.h>

namespace Phobos
{
	Button_c::Button_c(const String_c &up, const String_c &down, const String_c &update, Context_c *context):
		cmdUp(up),
		cmdDown(down),
		cmdUpdate(update),
		fValue(0)
	{		
		cmdUp.SetProc(PH_CONTEXT_CMD_BIND(&Button_c::CmdProc, this));
		cmdDown.SetProc(PH_CONTEXT_CMD_BIND(&Button_c::CmdProc, this));
		cmdUpdate.SetProc(PH_CONTEXT_CMD_BIND(&Button_c::CmdProc, this));

		if(context)
			this->Enable(*context);
	}

	void Button_c::Enable(Context_c &context)
	{
		context.AddContextCmd(cmdUp);
		context.AddContextCmd(cmdDown);
		context.AddContextCmd(cmdUpdate);	
	}

	void Button_c::Disable()
	{
		cmdUp.Unlink();
		cmdDown.Unlink();
		cmdUpdate.Unlink();

		fValue = 0;
	}

	void Button_c::CmdProc(const StringVector_t &args, Context_c & )
	{
		PH_ASSERT(args.size() >= 4);

		fValue = StringToFloat(args[3]);
	}

	AxisButton_c::AxisButton_c(const String_c &upA, const String_c &downA, const String_c &updateA, const String_c &upB, const String_c &downB, const String_c &updateB, Context_c *context):
		clButtonA(upA, downA, updateA, context),
		clButtonB(upB, downB, updateB, context)
	{
		//empty
	}

	void AxisButton_c::Enable(Context_c &context)
	{
		clButtonA.Enable(context);
		clButtonB.Enable(context);
	}

	void AxisButton_c::Disable()
	{
		clButtonA.Disable();
		clButtonB.Disable();
	}
}