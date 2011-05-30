/*
Phobos 3d
  February 2011

  Copyright (C) 2005-2010 Bruno Crivelari Sanches

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Bruno Crivelari Sanches bcsanches@gmail.com
*/

#include "PH_LinearMoverComponent.h"

#include <OgreEntity.h>

#include <PH_Dictionary.h>

#include "PH_DictionaryUtils.h"
#include "PH_EntityComponentFactory.h"
#include "PH_EntityKeys.h"

namespace Phobos
{
	PH_FULL_ENTITY_COMPONENT_CREATOR("LinearMover", LinearMoverComponent_c);

	PH_DEFINE_ENTITY_OUTPUT_MANAGER(LinearMoverComponent_c);

	PH_DEFINE_ENTITY_OUTPUT(LinearMoverComponent_c, PositionChanged);	
			
	LinearMoverComponent_c::LinearMoverComponent_c(const String_c &name):
		EntityComponent_c(name)
	{		
		//empty
	}

	LinearMoverComponent_c::~LinearMoverComponent_c()
	{
		//empty
	}

	void LinearMoverComponent_c::OnLoad(const Dictionary_c &dictionary)
	{
		//empty
	}
}