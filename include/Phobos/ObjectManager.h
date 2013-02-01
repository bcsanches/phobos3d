/*
Phobos 3d
January 2010
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


#ifndef PH_OBJECT_MANAGER_H
#define PH_OBJECT_MANAGER_H

#include "Phobos/DisableCopy.h"
#include "Phobos/BaseAPI.h"
#include "Phobos/Node.h"
#include "Phobos/PathFwd.h"

namespace Phobos
{
	namespace ObjectManager
	{
		PH_BASE_API void AddObject(Node &node, const Path &strPath);

		PH_BASE_API Node &LookupObject(const Path &path);

		PH_BASE_API void RemoveObject(Node &node);

		PH_BASE_API void Clear();

		PH_BASE_API bool IsEmpty();
	}
}

#endif
