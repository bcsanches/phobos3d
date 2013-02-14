/*
Phobos 3d
January 2010
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

#include "PH_Kernel.h"

#include "PH_Error.h"
#include "PH_Memory.h"

namespace Phobos
{
	Kernel_c *Kernel_c::pclInstance_g = 0;

	Kernel_c &Kernel_c::CreateInstance(const String_c &logName)
	{
		PH_ASSERT_MSG(pclInstance_g == 0, "[Kernel_c::CreateInstance] Instance already created");

		pclInstance_g = PH_NEW Kernel_c(logName);

		return *pclInstance_g;
	}

	Kernel_c &Kernel_c::GetInstance()
	{
		PH_ASSERT_MSG(pclInstance_g != 0, "[Kernel_c::GetInstance] Instance not created, call CreateInstance");

		return *pclInstance_g;
	}

	void Kernel_c::ReleaseInstance()
	{
		PH_ASSERT_MSG(pclInstance_g != 0, "[Kernel_c::GetInstance] Instance not created, call CreateInstance");

		delete pclInstance_g;
		pclInstance_g = 0;
	}

	Kernel_c::Kernel_c(const String_c &logName):
		clLog(logName, LOG_FLAG_COPY_TO_STDOUT)
	{
		//empty
	}
		
	Kernel_c::~Kernel_c()
	{
		//empty
	}

	void Kernel_c::AddLogListener(LogListener_c &listener)
	{
		clLog.AddListener(listener);
	}

	void Kernel_c::RemoveLogListener(LogListener_c &listener)
	{
		clLog.RemoveListener(listener);
	}
}


