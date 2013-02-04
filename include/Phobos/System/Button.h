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


#ifndef PH_SYSTEM_BUTTON_H
#define PH_SYSTEM_BUTTON_H

#include <Phobos/Types.h>
#include <Phobos/Shell/Command.h>
#include <Phobos/Shell/IContext.h>

#include "Phobos/System/SystemAPI.h"

namespace Phobos
{		
	namespace System
	{
		class PH_SYSTEM_API Button
		{
			public:
				Button(const String_t &up, const String_t &down, const String_t &update, Shell::IContext *context = NULL);		

				inline Float_t GetValue(void) const
				{
					return(fpValue);
				}

				void Enable(Phobos::Shell::IContext &context);
				void Disable();

			private:
				void CmdProc(const Shell::StringVector_t &args, Shell::Context & );

			private:
				Float_t fpValue;

				Shell::Command m_cmdUp;
				Shell::Command m_cmdDown;
				Shell::Command m_cmdUpdate;
		};
	}
}

#endif
