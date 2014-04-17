#ifndef PH_ENGINE_CLIENT_H
#define PH_ENGINE_CLIENT_H

#include "Phobos/Engine/EngineAPI.h"

#include "Phobos/Engine/IEscHandler.h"
#include "Phobos/Engine/IPlayerCommandProducer.h"

namespace Phobos
{
	namespace Engine
	{
		class PH_ENGINE_API Client : public IEscHandler
		{
			public:
				void Connect();
				void Disconnect();

				void FixedUpdate();
				void Update();

				virtual void DispatchCommand(IPlayerCmdPtr_t cmd) = 0;

			protected:
				virtual void OnConnect(){};
				virtual void OnDisconnect(){};

				virtual void OnFixedUpdate() {};
				virtual void OnUpdate() {};

				protected:
					Client();
		};
	}
}

#endif
