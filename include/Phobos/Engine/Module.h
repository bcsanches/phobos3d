/*
Phobos 3d
April 2010
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

#ifndef PH_CORE_MODULE_H
#define PH_CORE_MODULE_H

#include <Phobos/Node.h>

#include "Phobos/Engine/ModuleFwd.h"
#include "Phobos/Engine/EngineAPI.h"

namespace Phobos
{
	namespace Engine
	{
		class PH_ENGINE_API Module: public Node
		{
			public:
				void Update();
				void FixedUpdate();

				//The first step on initialization, nothing is loaded yet and all modules should be already created
				void PreInit();

				//CFG file has been loaded and executed, so handle any config changes
				void Init();

				//All modules applied settings and should be ready to go and receive commands
				void Start();		

				void Started();

				void Stop();
				void Finalize();			

			protected:
				explicit Module(const String_t &name, UInt32_t flags = 0);
				explicit Module(const Char_t *name, UInt32_t flags = 0);			

				virtual void OnUpdate() {}
				virtual void OnFixedUpdate() {}
				
				virtual void OnPreInit() {}				
				virtual void OnInit() {}
				virtual void OnStart() {}				
				virtual void OnStarted() {}

				virtual void OnFinalize() {}
				virtual void OnStop() {}				
		};

		typedef void (Module::*ModuleProc_t)();
	}
}

#endif
