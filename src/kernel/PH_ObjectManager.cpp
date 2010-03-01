/*
Phobos 3d
  January 2010

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

#include "PH_Exception.h"
#include "PH_ObjectManager.h"
#include "PH_Path.h"

namespace Phobos
{
	ObjectManager_c::ObjectManager_c():
		ipRoot(Node_c::Create("/"))
	{
	}

	void ObjectManager_c::AddObject(NodePtr_t ptr, const Path_c &strPath)
	{
		Path_c path(strPath);

		Path_c::Iterator_c it;

		//empty path?
		String_c currentFolder;
		if(!it.Init(path, &currentFolder))
		{
			ipRoot->AddChild(ptr);

			return;
		}

		NodePtr_t currentNode = ipRoot;
		for(;;)
		{
			NodePtr_t child;
			try
			{
				child = currentNode->GetChild(currentFolder);
			}
			catch(ObjectNotFoundException_c &)
			{			
				//No path, lets create a default one
				child = Node_c::Create(currentFolder);
				currentNode->AddChild(child);
			}
			currentNode = child.get();

			if(!it.GetNext(&currentFolder))
				break;
		}

		currentNode->AddChild(ptr);
	}

	NodePtr_t ObjectManager_c::LookupObject(const Path_c &path) const
	{
		Path_c::Iterator_c it;

		String_c folder;
		if(!it.Init(path, &folder))
		{
			std::stringstream stream;

			stream << "Invalid path: " << path;
			PH_RAISE(INVALID_PARAMETER_EXCEPTION, "ObjectManager_c::LookupObject", stream.str());
		}

		NodePtr_t currentNode = ipRoot;
		for(;;)
		{			
			NodePtr_t child = currentNode->GetChild(folder);
			currentNode = child.get();

			if(!it.GetNext(&folder))
				return child;	

			currentNode = child.get();
		}
	}

	void ObjectManager_c::RemoveObject(NodePtr_t ptr)
	{
		ptr->RemoveSelf();
	}
}
