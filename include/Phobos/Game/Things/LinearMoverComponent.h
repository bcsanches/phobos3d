/*
Phobos 3d
February 2011
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

#ifndef PH_LINEAR_MOVER_COMPONENT_H
#define PH_LINEAR_MOVER_COMPONENT_H

#include "Phobos/Game/Things/MoverComponent.h"
#include <Phobos/OgreEngine/TransformPropertyFwd.h>

namespace Phobos
{	
	namespace Game
	{
		namespace Things
		{
			class LinearMoverComponent: public MoverComponent
			{
				public:
					static std::unique_ptr<Component> Create(const String_t &name, Entity &owner);	

					~LinearMoverComponent();		

					virtual void FixedUpdate();

				protected:
					LinearMoverComponent(const String_t &name, Entity &owner);					

					virtual void OnLoad(const Register::Table &table);

				protected:
					//PH_DECLARE_ENTITY_OUTPUT_MANAGER;

				private:			
					//PH_DECLARE_ENTITY_OUTPUT(PositionChanged);

					OgreEngine::TransformProperty *m_pprpTransform;
			};
		}
	}
}


#endif