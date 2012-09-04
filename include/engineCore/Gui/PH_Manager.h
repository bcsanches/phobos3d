/*
Phobos 3d
September 2012
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

#ifndef PH_GUI_MANAGER_H
#define PH_GUI_MANAGER_H

#include <PH_Singleton.h>

#include "PH_CoreModule.h"
#include "PH_EngineCoreAPI.h"

#include "PH_ContextFwd.h"

namespace Phobos
{
	namespace Gui
	{
		PH_DECLARE_NODE_PTR(Manager);

		class PH_ENGINE_CORE_API Manager_c: public CoreModule_c
		{
			PH_DECLARE_SINGLETON_METHODS(Manager);

			public:
				ContextPtr_t CreateContext(const String_c &name);

			private:
				Manager_c();
		};
	}
}

#endif
