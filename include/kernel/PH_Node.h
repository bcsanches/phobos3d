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

#include "PH_DisableCopy.h"
#include "PH_NodeFwd.h"
#include "PH_NodeProperty.h"
#include "PH_Object.h"

namespace Phobos
{
	/**
		This node class is designed to allow all objects of the system to be structured like a filesystem.

		The major motivation to this is to allow easy scripting access and implementation of a property system.

		\remark Please note that this class was designed to be stored on the heap, we cannot easily block stack creation
		(using derived classes), but storing stack instances as child nodes results on undefined behavior.
	*/

	class Path_c;

	namespace NodeFlags
	{
		enum Enum
		{
			PRIVATE_CHILDREN = 0x01,
			MANAGED = 0x02
		};
	}	

	class PH_KERNEL_API Node_c: public Object_c
	{
		PH_DISABLE_COPY(Node_c);

		public:
			typedef std::map<String_c, Node_c *> NodeMap_t;
			typedef std::pair<String_c, Node_c *> NodeMapPair_t;

			typedef NodeMap_t::iterator iterator;
			typedef NodeMap_t::const_iterator const_iterator;

			typedef boost::intrusive::set<NodeProperty_c, boost::intrusive::constant_time_size<false> > NodePropertySet_t;

		public:			
			static NodePtr_t Create(const String_c &name);
			static NodePtr_t Create(const Char_t *name);
	
			~Node_c();
			explicit Node_c(const String_c &name, UInt32_t flags = 0);
			explicit Node_c(const Char_t *name, UInt32_t flags = 0);

			void AddChild(Node_c &node);
			void RemoveChild(Node_c &node);
			void RemoveAllChildren();
			Node_c &GetChild(const String_c &name) const;
			Node_c *TryGetChild(const String_c &name) const;

			Node_c &LookupNode(const Path_c &name) const;

			/**
				\returns: true if the process completed normally, false if an error ocurred. Note that a true
				does not means the value was found, check if \param result is null or not to confirm. When false is returned, an
				error ocurred, like an bad formed path


				If the returned value is true, but \param result is still null, this means that the object was not found, but the 
				request was processed without errors.
			*/
			bool TryLookupNode(Node_c *&result, const Path_c &name) const;

			void AddNode(Node_c &ptr, const Path_c &path);

			void RemoveSelf();

			size_t GetNumChildren() const;
			bool HasChildren() const;

			Node_c *GetParent() const;

			void GetThisPath(Path_c &out);

			inline NodeMap_t::const_iterator begin() const;
			inline NodeMap_t::const_iterator end() const;
			inline const NodeMap_t &GetNodes() const;	

			void AddProperty(NodeProperty_c &prop);
			void RemoveProperty(const char *name);

			NodeProperty_c &GetProperty(const char *name);

			template <typename T>
			inline T &GetCustomProperty(const char *name)
			{
				return static_cast<T &>(this->GetProperty(name));
			}			

		protected:			
			void AddPrivateChild(Node_c &node);
			void AddPrivateChild(std::unique_ptr<Node_c> &&ptr);

			inline NodeMap_t::iterator begin();
			inline NodeMap_t::iterator end();

			inline NodePtr_t MakePointerFromThis() const
			{
				//return NodePtr_t(const_cast<Node_c*>(this));
				return boost::static_pointer_cast<Node_c>(const_cast<Node_c *>(this)->shared_from_this());
			}

		private:
			void GetThisPath_r(Path_c &out);

			NodePropertySet_t::iterator GetPropertyIterator(const char *name);

			const Node_c *GetRoot() const;
			Node_c *GetRoot();

			inline void SetManaged(bool value);

		private:			
			NodePropertySet_t setProperties;

			NodeMap_t mapNodes;
			Node_c *pclParent;

			bool fPrivateChildren;
			bool fManagedNode;
	};

	inline Node_c::NodeMap_t::const_iterator Node_c::begin() const
	{
		return mapNodes.begin();
	}

	inline Node_c::NodeMap_t::const_iterator Node_c::end() const
	{
		return mapNodes.end();
	}

	inline Node_c::NodeMap_t::iterator Node_c::begin()
	{
		return mapNodes.begin();
	}

	inline Node_c::NodeMap_t::iterator Node_c::end()
	{
		return mapNodes.end();
	}

	inline const Node_c::NodeMap_t &Node_c::GetNodes() const
	{
		return mapNodes;
	}

	inline void Node_c::SetManaged(bool value)
	{
		fManagedNode = value;
	}

#ifdef PH_NODE_FOREACH

	inline Node_c::NodeMap_t::const_iterator range_begin( const Node_c &x )
    {
		return x.begin();
    }

    inline Node_c::NodeMap_t::const_iterator range_end( const Node_c &x )
    {
        return x.end();
    }
#endif

}

#ifdef PH_NODE_FOREACH
namespace boost
{    
    template<>
    struct range_const_iterator< Phobos::Node_c >
    {
		typedef Phobos::Node_c::NodeMap_t::const_iterator type;
    };
}
#endif

#endif
