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

#include <Phobos/Shell/Context.h>
#include <Phobos/Shell/Utils.h>
#include <Phobos/Error.h>


Phobos::System::Button::Button(const String_t &up, const String_t &down, const String_t &update, Shell::IContext *context):
	fpValue(0),
	m_cmdUp(up),
	m_cmdDown(down),
	m_cmdUpdate(update)
{
	m_cmdUp.SetProc(PH_CONTEXT_CMD_BIND(&Button::CmdProc, this));
	m_cmdDown.SetProc(PH_CONTEXT_CMD_BIND(&Button::CmdProc, this));
	m_cmdUpdate.SetProc(PH_CONTEXT_CMD_BIND(&Button::CmdProc, this));

	if(context)
		this->Enable(*context);
}

void Phobos::System::Button::Enable(Shell::IContext  &context)
{
	context.AddContextCommand(m_cmdUp);
	context.AddContextCommand(m_cmdDown);
	context.AddContextCommand(m_cmdUpdate);
}

void Phobos::System::Button::Disable()
{
	m_cmdUp.Unlink();
	m_cmdDown.Unlink();
	m_cmdUpdate.Unlink();

	fpValue = 0;
}

void Phobos::System::Button::CmdProc(const Shell::StringVector_t &args, Shell::Context & )
{
	PH_ASSERT(args.size() >= 4);

	fpValue = std::stof(args[3]);
}

