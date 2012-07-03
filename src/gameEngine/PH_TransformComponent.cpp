/*
Phobos 3d
June 2011
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

#include "PH_TransformComponent.h"

#include "PH_Entity.h"
#include "PH_EntityComponentFactory.h"
#include "PH_EntityUtils.h"

namespace Phobos
{
	PH_FULL_ENTITY_COMPONENT_CREATOR("Transform", TransformComponent_c);	

	TransformComponent_c::TransformComponent_c(const String_c &name, Entity_c &owner):
		EntityComponent_c(name, owner),
		propTransform("transform")
	{
		owner.AddProperty(propTransform);
	}

	TransformComponent_c::~TransformComponent_c()
	{
	}

	void TransformComponent_c::OnLoad(const Dictionary_c &dictionary)
	{			
		Transform_c transform;
		EntityLoadTransform(transform, dictionary);

		propTransform.SetTransform(transform);				
	}
}
