#include "PH_BaseClient.h"

#include <PH_Session.h>

namespace Phobos
{
	BaseClient::BaseClient(const char *name):
		Module(name)		
	{
		//empty
	}

	void BaseClient::OnPrepareToBoot()
	{
		Session::GetInstance().SetClient(this);
	}
}
