/*
Phobos 3d
June 2011
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

#ifndef PH_NODE_PROPERTY_H
#define PH_NODE_PROPERTY_H

#include <boost/intrusive/set.hpp>
#include <cstring>

#include "Phobos/Error.h"

namespace Phobos
{
	typedef boost::intrusive::set_base_hook<boost::intrusive::link_mode<boost::intrusive::auto_unlink> > NodePropertyAutoUnlinkHook_t;

	class NodeProperty: public NodePropertyAutoUnlinkHook_t
	{
		private:
			const char *m_pszName;

		protected:
			//Make sure name is a real const value, not a temporary or memory allocated string
			//The pointer will be stored, not a copy to the string
			NodeProperty(const char *name):
			  m_pszName(name)
			{
				PH_ASSERT(m_pszName);
			}
		public:

			inline bool operator<(const NodeProperty &rhs) const
			{
				if(m_pszName == rhs.m_pszName)
					return false;
				else
					return strcmp(m_pszName, rhs.m_pszName) < 0;
			}

			inline const char *GetName() const
			{
				return m_pszName;
			}
	};

	template <typename T>
	class NumericProperty: public NodeProperty
	{
		private:
			T tValue;

		public:
			NumericProperty(const char *name, T initialValue):
			  NodeProperty_c(name),
			  tValue(initialValue)
			{
				//empty
			}

			inline T GetValue() const
			{
				return tValue;
			}

			inline void SetValue(T value)
			{
				tValue = value;
			}
	};
}

#endif
