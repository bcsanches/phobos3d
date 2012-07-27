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

#include <PH_CoreModule.h>

#include "PH_GenericComponentManager.h"

namespace Phobos
{			
	template <typename T>
	class PH_GAME_ENGINE_API GenericComponentManagerModule_c: public CoreModule_c, public GenericComponentManager_c<T>
	{				
		protected:
			GenericComponentManagerModule_c(const String_c &name, ChildrenMode_e mode =PUBLIC_CHILDREN):
				CoreModule_c(name, mode)
			{
				//empty
			}
			
			~GenericComponentManagerModule_c()
			{
				//empty
			}		
	};
}

#endif

