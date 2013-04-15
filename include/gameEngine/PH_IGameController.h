#ifndef PH_IGAME_CONTROLLER_H
#define PH_IGAME_CONTROLLER_H

#include "PH_GameEngineAPI.h"

#include <Phobos/Engine/IPlayerCommandProducer.h>

namespace Phobos
{
	class PH_GAME_ENGINE_API IGameController
	{
		public:
			virtual ~IGameController() {}

			virtual void Update() {}
			virtual void FixedUpdate(Engine::IPlayerCmdPtr_t cmd)= 0;

			virtual void Enable() = 0;
			virtual void Disable() = 0;
	};
}

#endif
