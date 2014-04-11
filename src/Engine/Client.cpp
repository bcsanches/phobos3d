#include "Phobos/Engine/Client.h"

namespace Phobos
{
	namespace Engine
	{
		Client::Client()
		{
			//empty
		}

		void Client::Connect()
		{
			this->OnConnect();
		}

		void Client::Disconnect()
		{
			this->OnDisconnect();
		}

		void Client::FixedUpdate()
		{
			this->OnFixedUpdate();
		}

		void Client::Update()
		{
			this->OnUpdate();
		}
	}
}