/*
Phobos 3d
October 2011
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

#ifndef PH_RIGID_BODY_COMPONENT_H
#define PH_RIGID_BODY_COMPONENT_H

#include "Phobos/Game/Things/Component.h"
#include "Phobos/Game/GameAPI.h"
#include "Phobos/Game/Physics/Manager.h"
#include "Phobos/Game/Physics/RigidBody.h"

#include <Phobos/OgreEngine/Math/Transform.h>
#include <Phobos/OgreEngine/TransformPropertyFwd.h>

#define PH_RIGID_BODY_COMPONENT_NAME "RigidBody"

namespace Phobos
{	
	namespace Game
	{
		namespace Physics
		{		
			class PH_GAME_API RigidBodyComponent: public Things::Component
			{
				public:
					static std::unique_ptr<Things::Component> Create(const String_t &name, Things::Entity &owner);

					~RigidBodyComponent();

					void SaveTransform();
					void UpdateTransform(Float_t delta);

				protected:
					RigidBodyComponent(const String_t &name, Things::Entity &owner);				

					void OnLoad(const Register::Table &table);
					void OnLoadFinished();

				private:				
					RigidBody		m_clRigidBody;

					OgreEngine::TransformProperty		*m_pprpTransform;
					Engine::Math::Transform				m_clPreviousTransform;
			};
		}
	}
}

#endif
