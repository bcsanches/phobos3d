#ifndef PH_IGAME_CONTROLLER_H
#define PH_IGAME_CONTROLLER_H

#include "PH_GameEngineAPI.h"
#include "PH_IPlayerCommandProducer.h"

namespace Phobos
{
	class PH_GAME_ENGINE_API IGameController_c
	{
		public:
			virtual ~IGameController_c() {}

			virtual void Update() {}
			virtual void FixedUpdate(IPlayerCmdPtr_t cmd)= 0;

			virtual void Enable() = 0;
			virtual void Disable() = 0;
	};
}

#endif