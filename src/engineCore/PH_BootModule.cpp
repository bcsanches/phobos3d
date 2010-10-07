/*
Phobos 3d
  April 2010

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

#include "PH_BootModule.h"

#include <PH_Exception.h>
#include <PH_Kernel.h>

#include "PH_Console.h"
#include "PH_CoreModuleManager.h"

namespace Phobos
{
	BootModulePtr_t BootModule_c::Create(const String_c &cfgName, CoreModuleManager_c &manager)
	{
		return BootModulePtr_t(new BootModule_c(cfgName, manager));
	}

	BootModule_c::BootModule_c(const String_c &cfgName, CoreModuleManager_c &manager):
		CoreModule_c("BootModule"),
		strCfgName(cfgName),
		fUpdateDone(false),
		iFixedUpdateCount(0),
		fPrepareFired(false),
		fBootFired(false),
		rclManager(manager)
	{
	}

	void BootModule_c::OnFixedUpdate()
	{
		++iFixedUpdateCount;

		if(fUpdateDone && (iFixedUpdateCount > 2))
		{			
			//First time, tell the system that we are ready to go
			if(!fPrepareFired)
			{				
				rclManager.OnEvent(CORE_EVENT_PREPARE_TO_BOOT);
				fPrepareFired = true;

				//restart count
				iFixedUpdateCount = 0;
				fUpdateDone = 0;
			}
			else if(!fBootFired)
			{
				try
				{
					Console_c::GetInstance()->ExecuteFromFile(strCfgName);
				}
				catch(FileNotFoundException_c &e)
				{					
					Kernel_c::GetInstance().LogStream() << "[BootModule_c::OnFixedUpdate] Warning, boot failed: " << e.what();
				}

				//Time to boot and game over for us
				rclManager.OnEvent(CORE_EVENT_BOOT);
				
				fBootFired = true;

				rclManager.RemoveModule(*this);
			}
		}
	}

	void BootModule_c::OnUpdate()
	{
		fUpdateDone = true;
	}
}
