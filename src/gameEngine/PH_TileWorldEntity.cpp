/*
Phobos 3d
May 2012
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

#include "PH_TileWorldEntity.h"

#include "PH_EntityFactory.h"

namespace Phobos
{
	PH_FULL_ENTITY_CREATOR("TileWorldEntity", TileWorldEntity_c);

	TileWorldEntity_c::TileWorldEntity_c(const String_c &name):
		Entity_c(name)
	{
		//empty
	}

	void TileWorldEntity_c::Load(const MapLoader_c &loader)
	{

	}
}
