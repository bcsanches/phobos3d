/*
Phobos 3d
July 2012
Copyright (c) 2005-2012 Bruno Sanches  http://code.google.com/p/phobos3d

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef PH_GENERIC_COMPONENT_MANAGER_H
#define PH_GENERIC_COMPONENT_MANAGER_H

#include <Phobos/Exception.h>

#include "Phobos/Game/GameAPI.h"
#include "Phobos/Game/Things/HandleManager.h"
#include "Phobos/Game/WorldManager.h"

namespace Phobos
{			
	namespace Game
	{
		namespace Things
		{
			template <typename T>
			class PH_GAME_API GenericComponentManager
			{		
				public:
					GenericComponentManager()				
					{
						memset(m_arpclComponents, 0, sizeof(m_arpclComponents));
					}

					~GenericComponentManager()
					{
						//empty
					}

					void Register(T &comp)
					{
						UInt_t index = comp.GetEntityHandle().m_u12Index;
						if(m_arpclComponents[index] != NULL)
							PH_RAISE(INVALID_OPERATION_EXCEPTION, "[ModelRendererManager::Register]", "Slot already in use");

						m_arpclComponents[index] = &comp;
					}

					void Unregister(T &comp)
					{
						UInt_t index = comp.GetEntityHandle().m_u12Index;

						if(m_arpclComponents[index] != &comp)
							PH_RAISE(INVALID_PARAMETER_EXCEPTION, "[ModelRendererManager::Unregister]", "Slot contains another component");

						m_arpclComponents[index] = NULL;
					}			

					void CallForAll(void (T::*proc)())
					{
						UInt_t count = 0;
						UInt_t total = Phobos::Game::WorldManager::GetInstance().GetNumActiveEntities();

						for(int i = 0;i < MAX_HANDLES && count < total; ++i)
						{
							if(m_arpclComponents[i] == NULL)
								continue;

							(m_arpclComponents[i]->*proc)();
							++count;
						}
					}

					template <typename P1>
					void CallForAll1(void (T::*proc)(P1), P1 p1)
					{
						UInt_t count = 0;
						UInt_t total = WorldManager::GetInstance().GetNumActiveEntities();

						for(int i = 0;i < EntityManager::MAX_ENTRIES && count < total; ++i)
						{
							if(m_arpclComponents[i] == NULL)
								continue;

							(m_arpclComponents[i]->*proc)(p1);
							++count;
						}
					}			

				protected:
					T *m_arpclComponents[MAX_HANDLES];
			};
		}
	}
}

#endif

