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

#include "PH_Thumb.h"

#include <PH_Context.h>
#include <PH_ContextUtils.h>

namespace Phobos
{
	Thumb_c::Thumb_c(const String_c &cmd, IContext_c *context):
		cmdUpdate(cmd)
	{
		cmdUpdate.SetProc(PH_CONTEXT_CMD_BIND(&Thumb_c::CmdProc, this));

		fpPoint[0] = fpPoint[1] = 0;

		if(context)
			this->Enable(*context);		
	}

	void Thumb_c::Enable(IContext_c &context)
	{	
		context.AddContextCmd(cmdUpdate);	
	}

	void Thumb_c::Disable()
	{
		cmdUpdate.Unlink();
		fpPoint[0] = fpPoint[1] = 0;
	}


	void Thumb_c::CmdProc(const StringVector_t &args, Context_c &)
	{
		PH_ASSERT(args.size() >= 5);
		
		fpPoint[0] = StringToFloat(args[3]);
		fpPoint[1] = StringToFloat(args[4]);
	}
}