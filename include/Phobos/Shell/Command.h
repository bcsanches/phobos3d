/*
Phobos 3d
March 2010
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

#ifndef PH_SHELL_COMMAND_H
#define PH_SHELL_COMMAND_H

#include "Phobos/Shell/CommandFwd.h"
#include "Phobos/Shell/Item.h"

namespace Phobos
{	
	namespace Shell
	{
		class PH_BASE_API Command: public Item
		{	
			public:
				// =====================================================
				// PUBLIC METHODS
				// =====================================================
				inline Command(const String_t &name, CmdProc_t proc = CmdProc_t());		
				inline Command(const Command &other);

				inline void Execute(const StringVector_t &args, Context &context);

				inline void SetProc(CmdProc_t proc);

			protected:
				// =====================================================
				// PROTECTED ATTRIBUTES
				// =====================================================
				CmdProc_t		m_pfnProc;			
		};

		// =====================================================
		// INLINE IMPLEMENTATION
		// =====================================================

		inline Command::Command(const String_t &name, CmdProc_t proc):
			Item(name),
			m_pfnProc(proc)
		{
			//empty
		}

		inline Command::Command(const Command &other):
			Item(other),
			m_pfnProc(other.m_pfnProc)
		{
			//empty
		}

		inline void Command::SetProc(CmdProc_t proc)
		{
			m_pfnProc = proc;
		}

		inline void Command::Execute(const StringVector_t &args, Context &context)
		{	
			m_pfnProc(args, context);
		}
	}
}

#endif