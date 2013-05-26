#include "Phobos/Game/BaseClient.h"

#include <Phobos/Engine/Session.h>

Phobos::Game::BaseClient::BaseClient(const char *name):
	Module(name)		
{
	//empty
}

void Phobos::Game::BaseClient::OnPrepareToBoot()
{
	Engine::Session::GetInstance().SetClient(this);
}

