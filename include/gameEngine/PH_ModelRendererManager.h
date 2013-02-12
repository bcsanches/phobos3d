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

#ifndef PH_MODEL_RENDERER_MANAGER_H
#define PH_MODEL_RENDERER_MANAGER_H

#include "PH_GenericComponentManagerModule.h"

namespace Phobos
{		
	PH_DECLARE_SINGLETON_PTR(ModelRendererManager);

	class ModelRendererComponent;

	class PH_GAME_ENGINE_API ModelRendererManager: public GenericComponentManagerModule<ModelRendererComponent>
	{
		PH_DECLARE_SINGLETON_METHODS(ModelRendererManager);

		public:
			~ModelRendererManager();
		
		protected:
			ModelRendererManager();			

			virtual void OnUpdate();
	};
}

#endif