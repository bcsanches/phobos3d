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

#include "PH_Node.h"

#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>

#include "PH_Exception.h"
#include "PH_Path.h"

namespace Phobos
{
	struct NodePropertyComp_s
	{
		bool operator()(const char *name, const NodeProperty_c &rhs) const
		{
			if(name == rhs.GetName())
				return 0;
			else
				return strcmp(name, rhs.GetName()) < 0;
		}

		bool operator()(const NodeProperty_c &rhs, const char *name) const
		{
			if(name == rhs.GetName())
				return 0;
			else
				return strcmp(rhs.GetName(), name) < 0;
		}
	};

	NodePtr_t Node_c::Create(const String_c &name)
	{
		return NodePtr_t(boost::make_shared<Node_c>(name));
	}
			
	NodePtr_t Node_c::Create(const Char_t *name)
	{
		return NodePtr_t(boost::make_shared<Node_c>(name));
	}	

	Node_c::Node_c(const String_c &name, ChildrenMode_e param):
		Object_c(name),
		pclParent(NULL),
		fPrivateChildren(param == PRIVATE_CHILDREN ? true : false)
	{
	}

	Node_c::Node_c(const Char_t *name, ChildrenMode_e param):
		Object_c(name),
		pclParent(NULL),
		fPrivateChildren(param == PRIVATE_CHILDREN ? true : false)
	{
	}

	Node_c::~Node_c()
	{
		//We clear all nodes parents, so in case they are not destroyed no dangling pointers are left
		BOOST_FOREACH(NodeMapPair_t pair, mapNodes)
			pair.second->pclParent = NULL;
	}

	void Node_c::AddChild(NodePtr_t node)
	{
		if (this->fPrivateChildren)
			PH_RAISE(INVALID_OPERATION_EXCEPTION, "Node_c::AddChild", "Node " + this->GetName() + " has private childs");

		AddPrivateChild(node);
	}

	void Node_c::AddPrivateChild(NodePtr_t node)
	{
		if(node->pclParent)
		{
			std::stringstream stream;
			stream << "Node " << node->GetName() << " is already a child of " << node->pclParent->GetName() << ", unregister if first before adding it to " << this->GetName();
			PH_RAISE(INVALID_PARAMETER_EXCEPTION, "[Node_c::AddChild]", stream.str());
		} 
		else if(mapNodes.find(node->GetName()) != mapNodes.end())
		{
			std::stringstream stream;
			stream << "Node " << node->GetName() << " already exists on node " << this->GetName();
			PH_RAISE(OBJECT_ALREADY_EXISTS_EXCEPTION, "[Node_c::AddChild]", stream.str());
		}

		mapNodes.insert(std::make_pair(node->GetName(), node));
		node->pclParent = this;
	}

	void Node_c::RemoveSelf()
	{
		if(pclParent == NULL)
		{
			std::stringstream stream;
			stream << "Node " << this->GetName() << " does not have a parent";
			PH_RAISE(INVALID_PARAMETER_EXCEPTION, "[Node_c::RemoveSelf]", stream.str());
		}

		pclParent->RemoveChild(this->MakePointerFromThis());
	}

	void Node_c::RemoveAllChildren()
	{
		mapNodes.clear();
	}

	void Node_c::RemoveChild(NodePtr_t node)
	{
		//node is not registered?
		if(node->pclParent == NULL)
		{
			std::stringstream stream;
			stream << "Node " << node->GetName() << " is not registered, cant remove it from " << this->GetName();
			PH_RAISE(INVALID_PARAMETER_EXCEPTION, "[Node_c::RemoveChild]", stream.str());
		}
		//wrong parent?
		else if(node->pclParent != this)
		{
			std::stringstream stream;
			stream << "Node " << node->GetName() << " is not registered on " << this->GetName() << " it is registered on " << node->pclParent->GetName();
			PH_RAISE(INVALID_PARAMETER_EXCEPTION, "[Node_c::RemoveChild]", stream.str());
		}
				
		mapNodes.erase(node->GetName());
		node->pclParent = NULL;
	}

	NodePtr_t Node_c::TryGetChild(const String_c &name) const
	{
		if((name.size() == 1) && (name[0] == '.'))
			return this->MakePointerFromThis();
		else if((name.size() == 2) && (name.compare("..") == 0))
			return pclParent->MakePointerFromThis();

		NodeMap_t::const_iterator it = mapNodes.find(name);
		if(it == mapNodes.end())
		{
			return NodePtr_t();			
		}

		return it->second;
	}

	NodePtr_t Node_c::GetChild(const String_c &name) const
	{
		NodePtr_t ptr = this->TryGetChild(name);
		if(!ptr)
		{
			std::stringstream stream;
			stream << "Node " << name << " not found on node " << this->GetName();
			PH_RAISE(OBJECT_NOT_FOUND_EXCEPTION, "[Node_c::GetChild]", stream.str());
		}
		
		return ptr;
	}

	void Node_c::AddNode(NodePtr_t ptr, const Path_c &path)
	{
		if(!path.IsRelative() && pclParent)
		{
			this->GetRoot()->AddNode(ptr, path);
		}
		else
		{
			Path_c::Iterator_c it;

			//empty path?
			String_c currentFolder;
			if(!it.Init(path, &currentFolder))
			{
				this->AddChild(ptr);

				return;
			}

			NodePtr_t currentNode = this->MakePointerFromThis();
			for(;;)
			{
				NodePtr_t child = currentNode->TryGetChild(currentFolder);
				if(!child)
				{
					//No path, lets create a default one
					child = Node_c::Create(currentFolder);
					currentNode->AddChild(child);
				}
				currentNode = child;

				if(!it.GetNext(&currentFolder))
					break;
			}

			currentNode->AddChild(ptr);
		}
	}

	NodePtr_t Node_c::LookupNode(const Path_c &path) const
	{
		//If path is only "/"
		if(path.IsOnlyRoot())
			return this->GetRoot()->MakePointerFromThis();

		if(!path.IsRelative() && pclParent)
		{
			//A full path and this node is not the root, so delegate this to the root
			return this->GetRoot()->LookupNode(path);
		}		

		Path_c::Iterator_c it;

		String_c folder;
		if(!it.Init(path, &folder))
		{
			std::stringstream stream;

			stream << "Invalid path: " << path;
			PH_RAISE(INVALID_PARAMETER_EXCEPTION, "Node_c::LookupObject", stream.str());
		}

		NodePtr_t currentNode = this->MakePointerFromThis();
		for(;;)
		{			
			NodePtr_t child = currentNode->GetChild(folder);			
			if(!it.GetNext(&folder))
				return child;				

			currentNode = child;
		}
	}

	bool Node_c::TryLookupNode(NodePtr_t &result, const Path_c &path) const
	{
		result.reset();

		//If path is only "/"
		if(path.IsOnlyRoot())
		{
			result = this->GetRoot()->MakePointerFromThis();

			return true;
		}

		if(!path.IsRelative() && pclParent)
		{
			//A full path and this node is not the root, so delegate this to the root
			return this->GetRoot()->TryLookupNode(result, path);
		}		

		Path_c::Iterator_c it;

		String_c folder;
		if(!it.Init(path, &folder))
		{			
			//Invalid path, nothing todo, return a NULL
			return false;
		}

		NodePtr_t currentNode = this->MakePointerFromThis();
		for(;;)
		{			
			NodePtr_t child = currentNode->TryGetChild(folder);
			if(!child)
			{
				result = child;
				return true;
			}

			if(!it.GetNext(&folder))
			{
				result = child;
				
				return true;
			}

			currentNode = child;
		}		
	}

	const Node_c *Node_c::GetRoot() const
	{
		const Node_c *current = this;

		while(current->pclParent)
			current = current->pclParent;

		return current;
	}

	Node_c *Node_c::GetRoot()
	{
		return const_cast<Node_c *>(((const Node_c *)(this))->GetRoot());
	}

	size_t Node_c::GetNumChildren() const
	{
		return mapNodes.size();
	}

	bool Node_c::HasChildren() const
	{
		return !mapNodes.empty();
	}

	NodePtr_t Node_c::GetParent() const
	{
		return pclParent ? pclParent->MakePointerFromThis() : NodePtr_t();
	}

	void Node_c::GetThisPath(Path_c &out)
	{
		out.Clear();

		this->GetThisPath_r(out);
	}

	void Node_c::GetThisPath_r(Path_c &out)
	{
		if(pclParent != NULL)
			pclParent->GetThisPath_r(out);
		else		
		{
			out = "/";
		}

		out.AddName(this->GetName());
	}

	void Node_c::AddProperty(NodeProperty_c &prop)
	{
		NodePropertySet_t::iterator it = setProperties.lower_bound(prop.GetName(), NodePropertyComp_s());
		if((it != setProperties.end()) && (!setProperties.key_comp()(prop, *it)))
		{
			std::stringstream stream;
			stream << "Node " << this->GetName() << " already contains property named " << prop.GetName();

			PH_RAISE(OBJECT_ALREADY_EXISTS_EXCEPTION, "Node_c::AddProperty", stream.str());
		}

		setProperties.insert(it, prop);
	}

	Node_c::NodePropertySet_t::iterator Node_c::GetPropertyIterator(const char *name)
	{
		NodePropertySet_t::iterator it = setProperties.find(name, NodePropertyComp_s());
		if(it == setProperties.end())
		{
			std::stringstream stream;
			stream << "Node " << this->GetName() << " does not contains property named " << name;

			PH_RAISE(OBJECT_NOT_FOUND_EXCEPTION, "Node_c::AddProperty", stream.str());
		}

		return it;
	}
	
	void Node_c::RemoveProperty(const char *name)
	{		
		setProperties.erase(this->GetPropertyIterator(name));
	}

	NodeProperty_c &Node_c::GetProperty(const char *name)
	{
		return *(this->GetPropertyIterator(name));
	}
}
