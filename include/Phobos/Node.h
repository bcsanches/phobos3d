/*
Phobos 3d
January 2010
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


#ifndef PH_NODE_H
#define PH_NODE_H

//#define PH_NODE_FOREACH

#include <map>

#include <boost/intrusive/set.hpp>

#ifdef PH_NODE_FOREACH
#include <boost/range/iterator.hpp>
#endif

#include "Phobos/DisableCopy.h"
#include "Phobos/NodeFwd.h"
#include "Phobos/NodeProperty.h"
#include "Phobos/PathFwd.h"
#include "Phobos/Object.h"

namespace Phobos
{
	/**
		This node class is designed to allow all objects of the system to be structured like a filesystem.

		The major motivation to this is to allow easy scripting access and implementation of a property system.

		\remark Please note that this class was designed to be stored on the heap, we cannot easily block stack creation
		(using derived classes), but storing stack instances as child nodes results on undefined behavior.
	*/	
	enum NodeFlags
	{
		PRIVATE_CHILDREN = 0x01,
		MANAGED = 0x02		
	};

	class PH_BASE_API Node: public Object
	{
		PH_DISABLE_COPY(Node);

		public:
			typedef std::map<String_t, Node *> NodeMap_t;
			typedef std::pair<String_t, Node *> NodeMapPair_t;

			typedef NodeMap_t::iterator iterator;
			typedef NodeMap_t::const_iterator const_iterator;

			typedef boost::intrusive::set<NodeProperty, boost::intrusive::constant_time_size<false> > NodePropertySet_t;

		public:			
			static NodePtr_t Create(const String_t &name);
			static NodePtr_t Create(const Char_t *name);
	
			~Node();
			explicit Node(String_t &&name, UInt32_t flags = 0);
			explicit Node(const String_t &name, UInt32_t flags = 0);
			explicit Node(const Char_t *name, UInt32_t flags = 0);			

			void AddChild(Node &node);
			void AddChild(std::unique_ptr<Node> &&ptr);
			void RemoveChild(Node &node);
			void RemoveAllChildren();
			Node &GetChild(const String_t &name) const;
			Node *TryGetChild(const String_t &name) const;

			Node &LookupNode(const Path &name) const;

			/**
				\returns: true if the process completed normally, false if an error ocurred. Note that a true
				does not means the value was found, check if \param result is null or not to confirm. When false is returned, an
				error ocurred, like an bad formed path


				If the returned value is true, but \param result is still null, this means that the object was not found, but the 
				request was processed without errors.
			*/
			bool TryLookupNode(Node *&result, const Path &name) const;

			void AddNode(Node &ptr, const Path &path);

			void RemoveSelf();

			size_t GetNumChildren() const;
			bool HasChildren() const;

			Node *GetParent() const;

			void GetThisPath(Path &out);

			inline NodeMap_t::const_iterator begin() const;
			inline NodeMap_t::const_iterator end() const;
			inline const NodeMap_t &GetNodes() const;	

			void AddProperty(NodeProperty &prop);
			void RemoveProperty(const char *name);

			NodeProperty &GetProperty(const char *name);

			template <typename T>
			inline T &GetCustomProperty(const char *name)
			{
				return static_cast<T &>(this->GetProperty(name));
			}			

		protected:			
			void AddPrivateChild(Node &node);
			void AddPrivateChild(std::unique_ptr<Node> &&ptr);

			inline NodeMap_t::iterator begin();
			inline NodeMap_t::iterator end();

			inline NodePtr_t MakePointerFromThis() const
			{				
				return std::static_pointer_cast<Node>(const_cast<Node *>(this)->shared_from_this());
			}

		private:
			void GetThisPath_r(Path &out);

			NodePropertySet_t::iterator GetPropertyIterator(const char *name);

			const Node *GetRoot() const;
			Node *GetRoot();

			inline void SetManaged(bool value);

		private:			
			NodePropertySet_t m_setProperties;

			NodeMap_t m_mapNodes;
			Node *m_pclParent;

			bool m_fPrivateChildren;
			bool m_fManagedNode;
	};

	inline Node::NodeMap_t::const_iterator Node::begin() const
	{
		return m_mapNodes.begin();
	}

	inline Node::NodeMap_t::const_iterator Node::end() const
	{
		return m_mapNodes.end();
	}

	inline Node::NodeMap_t::iterator Node::begin()
	{
		return m_mapNodes.begin();
	}

	inline Node::NodeMap_t::iterator Node::end()
	{
		return m_mapNodes.end();
	}

	inline const Node::NodeMap_t &Node::GetNodes() const
	{
		return m_mapNodes;
	}

	inline void Node::SetManaged(bool value)
	{
		m_fManagedNode = value;
	}

#ifdef PH_NODE_FOREACH

	inline Node::NodeMap_t::const_iterator range_begin( const Node &x )
    {
		return x.begin();
    }

    inline Node::NodeMap_t::const_iterator range_end( const Node &x )
    {
        return x.end();
    }
#endif

}

#ifdef PH_NODE_FOREACH
namespace boost
{    
    template<>
    struct range_const_iterator< Phobos::Node >
    {
		typedef Phobos::Node::NodeMap_t::const_iterator type;
    };
}
#endif

#endif
