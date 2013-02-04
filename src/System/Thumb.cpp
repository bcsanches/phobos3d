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

#include "Phobos/System/Thumb.h"

#include <Phobos/Shell/IContext.h>
#include <Phobos/Shell/Utils.h>


Phobos::System::Thumb::Thumb(const String_t &cmd, Shell::IContext *context):
	m_cmdUpdate(cmd)
{
	m_cmdUpdate.SetProc(PH_CONTEXT_CMD_BIND(&Thumb::CmdProc, this));

	m_fpPoint[0] = m_fpPoint[1] = 0;

	if(context)
		this->Enable(*context);		
}

void Phobos::System::Thumb::Enable(Shell::IContext &context)
{	
	context.AddContextCommand(m_cmdUpdate);	
}

void Phobos::System::Thumb::Disable()
{
	m_cmdUpdate.Unlink();
	m_fpPoint[0] = m_fpPoint[1] = 0;
}


void Phobos::System::Thumb::CmdProc(const Shell::StringVector_t &args, Shell::Context &)
{
	PH_ASSERT(args.size() >= 5);
		
	m_fpPoint[0] = std::stof(args[3]);
	m_fpPoint[1] = std::stof(args[4]);
}
