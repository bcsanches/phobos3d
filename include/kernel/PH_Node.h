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

#ifndef PH_NODE_H
#define PH_NODE_H

#include <map>

#include "PH_NodeFwd.h"
#include "PH_Object.h"

namespace Phobos
{
	/**
		This node class is designed to allow all objects of the system to be structured like a filesystem.

		The major motivation to this is to allow easy scripting access and implementation of a property system.

		\remark Please note that this class was designed to be stored on the heap, we cannot easily block stack creation
		(using derived classes), but storing stack instances as child nodes results on undefined behavior.
	*/

	enum ChildrenMode_e
	{
	   PRIVATE_CHILDREN,
	   PUBLIC_CHILDREN
	};

	class PH_KERNEL_API Node_c: public Object_c
	{
		public:			
			static NodePtr_t Create(const String_c &name);
			static NodePtr_t Create(const Char_t *name);

			void AddChild(NodePtr_t node);
			void RemoveChild(NodePtr_t node);
			NodePtr_t GetChild(const String_c &name) const;
			NodePtr_t TryGetChild(const String_c &name) const;

			void RemoveSelf();

			size_t GetNumChildren() const;
			bool HasChildren() const;

			NodePtr_t GetParent() const;

		protected:
			explicit Node_c(const String_c &name, ChildrenMode_e=PUBLIC_CHILDREN);
			explicit Node_c(const Char_t *name, ChildrenMode_e=PUBLIC_CHILDREN);		
			~Node_c();

			void AddPrivateChild(NodePtr_t node);

		private:					
			typedef std::map<String_c, NodePtr_t> NodeMap_t;
			typedef std::pair<String_c, NodePtr_t> NodeMapPair_t;
			NodeMap_t mapNodes;
			Node_c *pclParent;

			bool fPrivateChildren;
	};
}

#endif
