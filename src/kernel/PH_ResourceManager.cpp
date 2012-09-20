/*
Phobos 3d
March 2012
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

#include "PH_ResourceManager.h"

#include "PH_Exception.h"
#include "PH_Path.h"

namespace Phobos
{
	ResourceManager_c::ResourceManager_c(const char *name):
		Node_c(name, NodeFlags::PRIVATE_CHILDREN)
	{
		//empty
	}

	ResourcePtr_t ResourceManager_c::LoadResource(const char *name)
	{
		Path_c path(name);

		if(!path.IsRelative())
		{
			std::stringstream stream;
			stream << "Only relatives paths are supported: " << name << ".";
			
			PH_RAISE(INVALID_PARAMETER_EXCEPTION, "[ResourceManager_c::LoadResource]", stream.str());
		}

		NodePtr_t resourceNode;

		PH_RAISE(INVALID_OPERATION_EXCEPTION, "[ResourceManager_c::LoadResource]", "Not implemented");

		//FIXME
		/*
		if(!this->TryLookupNode(resourceNode, path))
		{
			std::stringstream stream;
			stream << "Could not process load request for " << name << ".";

			//something bad happened, report it
			PH_RAISE(INVALID_PARAMETER_EXCEPTION, "[ResourceManager_c::LoadResource]", stream.str());
		}

		ResourcePtr_t resource;
		if(!resourceNode)
		{
			resource = this->CreateResource(path);

			Path_c resourcePath;
			path.ExtractPathAndFilename(&resourcePath, NULL);

			this->AddNode(resource, resourcePath);
		}
		else
		{
			resource = boost::dynamic_pointer_cast<Resource_c>(resourceNode);
			if(!resource)
			{
				std::stringstream stream;
				stream << "Resource " << name << " refers to a directory, not an object.";

				//something bad happened, report it
				PH_RAISE(INVALID_PARAMETER_EXCEPTION, "[ResourceManager_c::LoadResource]", stream.str());
			}
		}

		return resource;
		*/
	}
}
