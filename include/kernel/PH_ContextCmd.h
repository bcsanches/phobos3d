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

#ifndef PH_CONTEXT_CMD_H
#define PH_CONTEXT_CMD_H

#include "PH_ContextItem.h"
#include "PH_Defs.h"

#include <boost/function.hpp>
#include <vector>

namespace Phobos
{
	class Context_c;
	typedef std::vector<String_c> StringVector_t;
	typedef boost::function2<void, const StringVector_t &, Context_c &> CmdProc_t;

	class PH_KERNEL_API ContextCmd_c: public ContextItem_c
	{	
		public:
			// =====================================================
			// PUBLIC METHODS
			// =====================================================
			inline ContextCmd_c(const String_c &name, CmdProc_t proc = CmdProc_t());		
			inline ContextCmd_c(const ContextCmd_c &other);

			inline void Execute(const StringVector_t &args, Context_c &context);

			inline void SetProc(CmdProc_t proc);

		protected:
			// =====================================================
			// PROTECTED ATTRIBUTES
			// =====================================================
			CmdProc_t		pfnProc;			
	};

	// =====================================================
	// INLINE IMPLEMENTATION
	// =====================================================

	inline ContextCmd_c::ContextCmd_c(const String_c &name, CmdProc_t proc):
		ContextItem_c(name),
		pfnProc(proc)
	{
		//empty
	}

	inline ContextCmd_c::ContextCmd_c(const ContextCmd_c &other):
		ContextItem_c(other),
		pfnProc(other.pfnProc)
	{
		//empty
	}

	inline void ContextCmd_c::SetProc(CmdProc_t proc)
	{
		pfnProc = proc;
	}

	inline void ContextCmd_c::Execute(const StringVector_t &args, Context_c &context)
	{	
		pfnProc(args, context);
	}
}

#endif