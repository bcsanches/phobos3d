/*
Phobos 3d
March 2010
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
#ifndef PH_SHELL_ITEM_H
#define PH_SHELL_ITEM_H

#include <boost/intrusive/set.hpp>

#include "Phobos/BaseAPI.h"
#include "Phobos/String.h"

namespace Phobos
{
	namespace Shell
	{
		typedef boost::intrusive::set_base_hook<boost::intrusive::link_mode<boost::intrusive::auto_unlink> > ItemAutoUnlinkHook_t;

		class PH_BASE_API Item: public ItemAutoUnlinkHook_t
		{		
			public:	
				inline Item(const String_t &name);
				virtual ~Item(void) {};			

				inline const String_t &GetName(void) const;			

				inline void Unlink();

				inline bool operator<(const Item &rhs) const;

			private:	
				String_t m_strName;			
		};

		inline Item::Item(const String_t &name):
			m_strName(name)
		{
			//empty
		}	

		inline const String_t &Item::GetName(void) const
		{
			return(m_strName);
		}	

		inline bool Item::operator<(const Item &rhs) const
		{
			return m_strName.compare(rhs.m_strName) < 0;
		}

		inline void Item::Unlink()
		{
			unlink();
		}
	}
}

#endif
