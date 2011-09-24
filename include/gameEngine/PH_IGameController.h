#ifndef PH_IGAME_CONTROLLER_H
#define PH_IGAME_CONTROLLER_H

#include "PH_GameEngineAPI.h"

namespace Phobos
{
	class PH_GAME_ENGINE_API IGameController_c
	{
		public:
			virtual ~IGameController_c() {}

			virtual void Update() {}
			virtual void FixedUpdate() {}

			virtual void Enable() = 0;
			virtual void Disable() = 0;
	};
}

#endif