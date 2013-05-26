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

#ifndef PH_GENERIC_COMPONENT_MANAGER_MODULE_H
#define PH_GENERIC_COMPONENT_MANAGER_MODULE_H

#include <Phobos/Engine/Module.h>

#include "GenericComponentManager.h"

namespace Phobos
{			
	namespace Game
	{
		namespace Things
		{
			template <typename T>
			class PH_GAME_API GenericComponentManagerModule: public Engine::Module, public GenericComponentManager<T>
			{				
				protected:
					GenericComponentManagerModule(const String_t &name, UInt32_t flags =0):
						Module(name, flags)
					{
						//empty
					}
			
					~GenericComponentManagerModule()
					{
						//empty
					}		
			};
		}
	}
}

#endif

