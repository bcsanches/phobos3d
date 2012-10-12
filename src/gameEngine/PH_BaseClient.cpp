#include "PH_BaseClient.h"

#include <PH_Session.h>

namespace Phobos
{
	BaseClient_c::BaseClient_c(const char *name):
		CoreModule_c(name)		
	{
		//empty
	}

	void BaseClient_c::OnPrepareToBoot()
	{
		Session_c::GetInstance().SetClient(this);
	}
}
