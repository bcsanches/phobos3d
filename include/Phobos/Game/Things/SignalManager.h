/*
Phobos 3d
July 2011
Copyright (c) 2005-2011 Bruno Sanches  http://code.google.com/p/phobos3d

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef PH_SIGNAL_MANAGER_H
#define PH_SIGNAL_MANAGER_H

#include <map>

#include <Phobos/Engine/Module.h>
#include <Phobos/Singleton.h>

#include "Phobos/Game/GameAPI.h"

#include "Phobos/Game/Things/Signal.h"

#define PH_MAKE_SIGNAL_PROC(X) static_cast<Phobos::Game::Things::Signal::SignalProc_t>(X)

namespace Phobos
{
	namespace Game
	{		
		namespace Things
		{
			PH_DECLARE_SINGLETON_PTR(SignalManager);

			class PH_GAME_API SignalManager: public Engine::Module
			{
				PH_DECLARE_SINGLETON_METHODS(SignalManager);

				public:			
					~SignalManager();

					void Schedule(Thing &receiver, Signal::SignalProc_t proc, Float_t delay);

					void CancelEvents(const Thing &receiver);

				protected:
					virtual void OnFixedUpdate();

				private:
					SignalManager();			

				private:
					typedef std::multimap<Float_t, Signal> SignalMultiMap_t;

					SignalMultiMap_t m_mmapSignals;
			};
		}
	}
}

#endif
