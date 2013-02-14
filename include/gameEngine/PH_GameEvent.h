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

#ifndef PH_GAME_EVENT_H
#define PH_GAME_EVENT_H

#include <Phobos/Types.h>

#include "PH_GameEngineAPI.h"
#include "PH_EntityIO.h"

#define PH_MAKE_EVENT_PROC(X) reinterpret_cast<Phobos::GameEvent::EventProc_t>(X)

namespace Phobos
{	
	class PH_GAME_ENGINE_API GameEvent
	{
		public:
			typedef void (EntityIO::*EventProc_t)();

		public:
			GameEvent(EntityIO &receiver, EventProc_t proc);		

			void Fire();

			inline const EntityIO &GetReceiver() const;

		private:
			EntityIO &m_rclReceiver;
			EventProc_t m_pfnProc;
	};

	inline const EntityIO &GameEvent::GetReceiver() const
	{
		return m_rclReceiver;
	}
}

#endif