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

#include "Phobos/ResourceManager.h"

#include "Phobos/Exception.h"
#include "Phobos/Path.h"

namespace Phobos
{
	ResourceManager::ResourceManager(const char *name):
		Node(name, NodeFlags::PRIVATE_CHILDREN)
	{
		//empty
	}

	ResourcePtr_t ResourceManager::LoadResource(const char *name)
	{
		Path path(name);

		if(!path.IsRelative())
		{
			std::stringstream stream;
			stream << "Only relatives paths are supported: " << name << ".";
			
			PH_RAISE(INVALID_PARAMETER_EXCEPTION, "[ResourceManager::LoadResource]", stream.str());
		}

		NodePtr_t resourceNode;

		PH_RAISE(INVALID_OPERATION_EXCEPTION, "[ResourceManager::LoadResource]", "Not implemented");

		//FIXME
		/*
		if(!this->TryLookupNode(resourceNode, path))
		{
			std::stringstream stream;
			stream << "Could not process load request for " << name << ".";

			//something bad happened, report it
			PH_RAISE(INVALID_PARAMETER_EXCEPTION, "[ResourceManager::LoadResource]", stream.str());
		}

		ResourcePtr_t resource;
		if(!resourceNode)
		{
			resource = this->CreateResource(path);

			Path resourcePath;
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
				PH_RAISE(INVALID_PARAMETER_EXCEPTION, "[ResourceManager::LoadResource]", stream.str());
			}
		}

		return resource;
		*/
	}
}
