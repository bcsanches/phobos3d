/*
Phobos 3d
January 2010
Copyright (c) 2005-2013 Bruno Sanches  http://code.google.com/p/phobos3d

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef PH_EVENT_MANAGER_MODULE_H
#define PH_EVENT_MANAGER_MODULE_H

#include "PH_CoreModule.h"
#include "PH_Core.h"
#include <Phobos/System/EventManager.h>
#include <PH_Error.h>

namespace Phobos
{
	PH_DECLARE_NODE_PTR(EventManagerModule);	

	class PH_ENGINE_CORE_API EventManagerModule_c: public CoreModule_c, private Phobos::System::EventListener_c
	{
		public:
			static EventManagerModule_c &CreateInstance();
			static void ReleaseInstance();
			static EventManagerModule_c &GetInstance();

			~EventManagerModule_c();

		private:

			void OnFixedUpdate();			

			virtual void Event(struct System::Event_s &event);

			EventManagerModule_c();			

		private:			
			static EventManagerModulePtr_t ipInstance_gl;

	};
}
#endif
