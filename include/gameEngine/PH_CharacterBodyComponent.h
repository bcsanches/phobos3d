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

#include <PH_Vector3Property.h>

#include "PH_EntityComponent.h"
#include "PH_GameEngineAPI.h"
#include "PH_PhysicsManager.h"
#include "PH_Transform.h"

#define PH_CHARACTER_BODY_COMPONENT_NAME "CharacterBody"

namespace Phobos
{
	class TransformProperty_c;

	namespace Physics
	{
		class PH_GAME_ENGINE_API CharacterBodyComponent_c: public EntityComponent_c
		{
			public:
				static EntityComponent_c *Create(const String_c &name, Entity_c &owner);

				~CharacterBodyComponent_c();

				void PreparePhysicsFrame(Float_t delta);
				void FinishPhysicsFrame();

			protected:
				CharacterBodyComponent_c(const String_c &name, Entity_c &owner);				

				void OnLoad(const Register::Table_c &table);
				void OnLoadFinished();

			private:				
				Vector3Property_c	prpVelocity;
				Vector3Property_c	prpCharacterPosition;

				CharacterBodyPtr_t	spCharacterBody;

				TransformProperty_c *pprpTransform;				
		};
	}
}

#endif