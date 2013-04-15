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

#include "PH_LinearMoverComponent.h"

#include <OgreEntity.h>

#include <Phobos/Register/Table.h>

#include <Phobos/OgreEngine/TransformProperty.h>

#include "PH_GameRegisterUtils.h"
#include "PH_EntityComponentFactory.h"
#include "PH_EntityKeys.h"

namespace Phobos
{
	PH_FULL_ENTITY_COMPONENT_CREATOR("LinearMover", LinearMoverComponent);

	PH_DEFINE_ENTITY_OUTPUT_MANAGER(LinearMoverComponent);

	PH_DEFINE_ENTITY_OUTPUT(LinearMoverComponent, PositionChanged);	
		
	LinearMoverComponent::LinearMoverComponent(const String_t &name, Entity &owner):
		MoverComponent(name, owner)
	{		
		//empty
	}

	LinearMoverComponent::~LinearMoverComponent()
	{
		//empty
	}

	void LinearMoverComponent::OnLoad(const Register::Table &table)
	{
		m_pprpTransform = &this->GetCustomEntityProperty<OgreEngine::TransformProperty>(PH_ENTITY_PROP_TRANSFORM);
	}

	void LinearMoverComponent::FixedUpdate()
	{		
		m_pprpTransform->SetOrigin(m_pprpTransform->GetOrigin() + Ogre::Vector3(0.01f, 0, 0));
	}
}
