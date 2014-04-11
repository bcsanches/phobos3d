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

#include "Phobos/Node.h"

#include "Phobos/Exception.h"
#include "Phobos/Memory.h"
#include "Phobos/Path.h"

namespace Phobos
{
	struct NodePropertyComp_s
	{
		bool operator()(const char *name, const NodeProperty &rhs) const
		{
			if(name == rhs.GetName())
				return 0;
			else
				return strcmp(name, rhs.GetName()) < 0;
		}

		bool operator()(const NodeProperty &rhs, const char *name) const
		{
			if(name == rhs.GetName())
				return 0;
			else
				return strcmp(rhs.GetName(), name) < 0;
		}
	};

	NodePtr_t Node::Create(const String_t &name)
	{
		return NodePtr_t(std::make_shared<Node>(name));
	}
			
	NodePtr_t Node::Create(const Char_t *name)
	{
		return NodePtr_t(std::make_shared<Node>(name));
	}	

	Node::Node(String_t &&name, UInt32_t flags) :
		Object(name),
		m_pclParent(NULL),
		m_fPrivateChildren(flags & NodeFlags::PRIVATE_CHILDREN ? true : false),
		m_fManagedNode(flags & NodeFlags::MANAGED ? true : false)
	{
		//empty
	}

	Node::Node(const String_t &name, UInt32_t flags):
		Object(name),
		m_pclParent(NULL),
		m_fPrivateChildren(flags & NodeFlags::PRIVATE_CHILDREN ? true : false),
		m_fManagedNode(flags & NodeFlags::MANAGED ? true : false)
	{
		//empty
	}

	Node::Node(const Char_t *name, UInt32_t flags):
		Object(name),
		m_pclParent(NULL),
		m_fPrivateChildren(flags & NodeFlags::PRIVATE_CHILDREN ? true : false),
		m_fManagedNode(flags & NodeFlags::MANAGED ? true : false)
	{
		//empty
	}

	Node::~Node()
	{
		if(m_pclParent != NULL)
			m_pclParent->RemoveChild(*this);

		this->RemoveAllChildren();
	}	

	void Node::AddChild(Node &node)
	{
		if (m_fPrivateChildren)
			PH_RAISE(INVALID_OPERATION_EXCEPTION, "Node::AddChild", "Node " + this->GetName() + " has private childs");

		AddPrivateChild(node);
	}

	void Node::AddChild(std::unique_ptr<Node> &&ptr)
	{
		if (m_fPrivateChildren)
			PH_RAISE(INVALID_OPERATION_EXCEPTION, "Node::AddChild", "Node " + this->GetName() + " has private childs");

		AddPrivateChild(std::move(ptr));
	}

	Node &Node::AddPrivateChild(Node &node)
	{
		if(node.m_pclParent)
		{
			std::stringstream stream;
			stream << "Node " << node.GetName() << " is already a child of " << node.m_pclParent->GetName() << ", unregister if first before adding it to " << this->GetName();
			PH_RAISE(INVALID_PARAMETER_EXCEPTION, "[Node::AddChild]", stream.str());
		} 
		else if(m_mapNodes.find(node.GetName()) != m_mapNodes.end())
		{
			std::stringstream stream;
			stream << "Node " << node.GetName() << " already exists on node " << this->GetName();
			PH_RAISE(OBJECT_ALREADY_EXISTS_EXCEPTION, "[Node::AddChild]", stream.str());
		}

		m_mapNodes.insert(std::make_pair(node.GetName(), &node));
		node.m_pclParent = this;

		return node;
	}

	Node &Node::AddPrivateChild(std::unique_ptr<Node> &&ptr)
	{
		ptr->SetManaged(true);
		auto &node = this->AddPrivateChild(*ptr);
		
		ptr.release();

		return node;
	}

	void Node::RemoveSelf()
	{
		if(m_pclParent == NULL)
		{
			std::stringstream stream;
			stream << "Node " << this->GetName() << " does not have a parent";
			PH_RAISE(INVALID_PARAMETER_EXCEPTION, "[Node::RemoveSelf]", stream.str());
		}

		m_pclParent->RemoveChild(*this);
	}

	void Node::RemoveAllChildren()
	{
		//We clear all nodes parents, so in case they are not destroyed no dangling pointers are left
		for(NodeMapPair_t pair : m_mapNodes)
		{
			pair.second->m_pclParent = NULL;
			if(pair.second->m_fManagedNode)
				delete pair.second;
		}

		m_mapNodes.clear();
	}

	void Node::RemoveChild(Node &node)
	{
		//node is not registered?
		if(node.m_pclParent == NULL)
		{
			std::stringstream stream;
			stream << "Node " << node.GetName() << " is not registered, cant remove it from " << this->GetName();
			PH_RAISE(INVALID_PARAMETER_EXCEPTION, "[Node::RemoveChild]", stream.str());
		}
		//wrong parent?
		else if(node.m_pclParent != this)
		{
			std::stringstream stream;
			stream << "Node " << node.GetName() << " is not registered on " << this->GetName() << " it is registered on " << node.m_pclParent->GetName();
			PH_RAISE(INVALID_PARAMETER_EXCEPTION, "[Node::RemoveChild]", stream.str());
		}
				
		m_mapNodes.erase(node.GetName());
		node.m_pclParent = NULL;
	}

	Node *Node::TryGetChild(const String_t &name) const
	{
		if((name.size() == 1) && (name[0] == '.'))
			return const_cast<Node *>(this);
		else if((name.size() == 2) && (name.compare("..") == 0))
			return m_pclParent;

		NodeMap_t::const_iterator it = m_mapNodes.find(name);
		if(it == m_mapNodes.end())
		{
			return NULL;			
		}

		return it->second;
	}

	Node &Node::GetChild(const String_t &name) const
	{
		Node *ptr = this->TryGetChild(name);
		if(!ptr)
		{
			std::stringstream stream;
			stream << "Node " << name << " not found on node " << this->GetName();
			PH_RAISE(OBJECT_NOT_FOUND_EXCEPTION, "[Node::GetChild]", stream.str());
		}
		
		return *ptr;
	}

	void Node::AddNode(Node &ptr, const Path &path)
	{
		if(!path.IsRelative() && m_pclParent)
		{
			this->GetRoot()->AddNode(ptr, path);
		}
		else
		{
			Path::Iterator it;

			//empty path?
			String_t currentFolder;
			if(!it.Init(path, &currentFolder))
			{
				this->AddChild(ptr);

				return;
			}

			Node *currentNode = this;
			for(;;)
			{
				Node *child = currentNode->TryGetChild(currentFolder);
				if(!child)
				{
					//No path, lets create a default one					
					std::unique_ptr<Node> localChild(PH_NEW Node(currentFolder, NodeFlags::MANAGED));

					child = localChild.get();

					currentNode->AddChild(std::move(localChild));
				}
				currentNode = child;

				if(!it.GetNext(&currentFolder))
					break;
			}

			currentNode->AddChild(ptr);
		}
	}

	Node &Node::LookupNode(const Path &path) const
	{
		//If path is only "/"
		if(path.IsOnlyRoot())
			return const_cast<Node &>(*(this->GetRoot()));

		if(!path.IsRelative() && m_pclParent)
		{
			//A full path and this node is not the root, so delegate this to the root
			return this->GetRoot()->LookupNode(path);
		}		

		Path::Iterator it;

		String_t folder;
		if(!it.Init(path, &folder))
		{
			std::stringstream stream;

			stream << "Invalid path: " << path;
			PH_RAISE(INVALID_PARAMETER_EXCEPTION, "Node::LookupObject", stream.str());
		}

		Node *currentNode = const_cast<Node *>(this);
		for(;;)
		{			
			Node &child = currentNode->GetChild(folder);			
			if(!it.GetNext(&folder))
				return child;			

			currentNode = &child;
		}
	}

	bool Node::TryLookupNode(Node *&result, const Path &path) const
	{
		result = NULL;

		//If path is only "/"
		if(path.IsOnlyRoot())
		{
			result = const_cast<Node *>(this->GetRoot());

			return true;
		}

		if(!path.IsRelative() && m_pclParent)
		{
			//A full path and this node is not the root, so delegate this to the root
			return this->GetRoot()->TryLookupNode(result, path);
		}		

		Path::Iterator it;

		String_t folder;
		if(!it.Init(path, &folder))
		{			
			//Invalid path, nothing todo, return a NULL
			return false;
		}

		Node *currentNode = const_cast<Node *>(this);
		for(;;)
		{			
			Node *child = currentNode->TryGetChild(folder);
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

	const Node *Node::GetRoot() const
	{
		const Node *current = this;

		while(current->m_pclParent)
			current = current->m_pclParent;

		return current;
	}

	Node *Node::GetRoot()
	{
		return const_cast<Node *>(((const Node *)(this))->GetRoot());
	}

	size_t Node::GetNumChildren() const
	{
		return m_mapNodes.size();
	}

	bool Node::HasChildren() const
	{
		return !m_mapNodes.empty();
	}

	Node *Node::GetParent() const
	{
		return m_pclParent;
	}

	void Node::GetThisPath(Path &out)
	{
		out.Clear();

		this->GetThisPath_r(out);
	}

	void Node::GetThisPath_r(Path &out)
	{
		if(m_pclParent != NULL)
			m_pclParent->GetThisPath_r(out);
		else		
		{
			out = "/";
		}

		out.AddName(this->GetName());
	}

	void Node::AddProperty(NodeProperty &prop)
	{
		NodePropertySet_t::iterator it = m_setProperties.lower_bound(prop.GetName(), NodePropertyComp_s());
		if((it != m_setProperties.end()) && (!m_setProperties.key_comp()(prop, *it)))
		{
			std::stringstream stream;
			stream << "Node " << this->GetName() << " already contains property named " << prop.GetName();

			PH_RAISE(OBJECT_ALREADY_EXISTS_EXCEPTION, "Node::AddProperty", stream.str());
		}

		m_setProperties.insert(it, prop);
	}

	Node::NodePropertySet_t::iterator Node::GetPropertyIterator(const char *name)
	{
		NodePropertySet_t::iterator it = m_setProperties.find(name, NodePropertyComp_s());
		if(it == m_setProperties.end())
		{
			std::stringstream stream;
			stream << "Node " << this->GetName() << " does not contains property named " << name;

			PH_RAISE(OBJECT_NOT_FOUND_EXCEPTION, "Node::AddProperty", stream.str());
		}

		return it;
	}
	
	void Node::RemoveProperty(const char *name)
	{		
		m_setProperties.erase(this->GetPropertyIterator(name));
	}

	NodeProperty &Node::GetProperty(const char *name)
	{
		return *(this->GetPropertyIterator(name));
	}
}
