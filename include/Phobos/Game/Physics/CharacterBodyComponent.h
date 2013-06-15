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

#ifndef PH_CHARACTER_BODY_COMPONENT_H
#define PH_CHARACTER_BODY_COMPONENT_H

#include <Phobos/OgreEngine/Vector3Property.h>
#include <Phobos/OgreEngine/TransformPropertyFwd.h>

#include "Phobos/Game/Things/Component.h"
#include "Phobos/Game/GameAPI.h"
#include "Phobos/Game/Physics/Manager.h"

#define PH_CHARACTER_BODY_COMPONENT_NAME "CharacterBody"

namespace Phobos
{	
	namespace Game
	{
		namespace Physics
		{
			class PH_GAME_API CharacterBodyComponent: public Things::Component
			{
				public:
					static std::unique_ptr<Component> Create(const String_t &name, Things::Entity &owner);

					~CharacterBodyComponent();

					void PreparePhysicsFrame(Float_t delta);
					void FinishPhysicsFrame();

				protected:
					CharacterBodyComponent(const String_t &name, Things::Entity &owner);				

					void OnLoad(const Register::Table &table);
					void OnLoadFinished();

				private:				
					OgreEngine::Vector3Property	m_prpVelocity;
					OgreEngine::Vector3Property	m_prpCharacterPosition;

					CharacterBodyPtr_t	m_spCharacterBody;

					OgreEngine::TransformProperty *m_pprpTransform;				
			};
		}
	}
}

#endif
