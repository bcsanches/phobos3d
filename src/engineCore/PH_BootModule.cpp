#include "PH_BootModule.h"

#include "PH_Core.h"

namespace Phobos
{
	BootModulePtr_t BootModule_c::Create()
	{
		return BootModulePtr_t(new BootModule_c());
	}

	BootModule_c::BootModule_c():
		CoreModule_c("BootModule"),
		fUpdateDone(false),
		iFixedUpdateCount(0)		
	{
	}

	void BootModule_c::OnFixedUpdate()
	{
		++iFixedUpdateCount;

		if(fUpdateDone && (iFixedUpdateCount > 2))
		{
			Core_c::GetInstance()->AddModuleToDestroyList(*this);
		}
	}

	void BootModule_c::OnUpdate()
	{
		fUpdateDone = true;
	}
}
