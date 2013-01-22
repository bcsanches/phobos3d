/*
Phobos 3d
September 2010
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

#include "PH_ProcVector.h"

#include <boost/foreach.hpp>

namespace Phobos
{
	void ProcVector_c::AddProc(Proc_t proc)
	{
		vecProcs.push_back(proc);
	}

	void ProcVector_c::CallAll()
	{
		BOOST_REVERSE_FOREACH(Proc_t proc, vecProcs)
		{
			proc();
		}
	}
			
	void ProcVector_c::Clear()
	{
		vecProcs.clear();
	}
}

