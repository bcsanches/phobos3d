#include "PH_BaseClient.h"

#include <Phobos/Engine/Session.h>

namespace Phobos
{
	BaseClient::BaseClient(const char *name):
		Module(name)		
	{
		//empty
	}

	void BaseClient::OnPrepareToBoot()
	{
		Engine::Session::GetInstance().SetClient(this);
	}
}
