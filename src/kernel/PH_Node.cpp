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

#include "PH_Node.h"

#include <boost/foreach.hpp>

#include "PH_Exception.h"

namespace Phobos
{
	NodePtr_t Node_c::Create(const String_c &name)
	{
		return NodePtr_t(new Node_c(name));
	}
			
	NodePtr_t Node_c::Create(const Char_t *name)
	{
		return NodePtr_t(new Node_c(name));
	}

	Node_c::Node_c(const String_c &name):
		Object_c(name),
		pclParent(NULL)
	{
	}

	Node_c::Node_c(const Char_t *name):
		Object_c(name),
		pclParent(NULL)
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

		pclParent->RemoveChild(NodePtr_t(this));
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
		return NodePtr_t(pclParent);
	}
}
