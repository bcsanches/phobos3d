#ifndef PH_IGAME_CONTROLLER_H
#define PH_IGAME_CONTROLLER_H

#include "Phobos/Game/GameAPI.h"

#include <Phobos/Engine/IPlayerCommandProducer.h>

namespace Phobos
{
	namespace Game
	{
		class PH_GAME_API IController
		{
			public:
				virtual ~IController() {}

				virtual void Update() {}
				virtual void FixedUpdate(Engine::IPlayerCmdPtr_t cmd)= 0;

				virtual void Enable() = 0;
				virtual void Disable() = 0;
		};
	}
}

#endif
