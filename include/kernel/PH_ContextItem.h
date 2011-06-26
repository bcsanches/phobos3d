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
#ifndef PH_CONTEXT_ITEM_H
#define PH_CONTEXT_ITEM_H

#include <boost/intrusive/set.hpp>

#include "PH_KernelAPI.h"
#include "PH_String.h"

namespace Phobos
{
	typedef boost::intrusive::set_base_hook<boost::intrusive::link_mode<boost::intrusive::auto_unlink> > ContextItemAutoUnlinkHook_t;

	class PH_KERNEL_API ContextItem_c: public ContextItemAutoUnlinkHook_t
	{		
		public:	
			inline ContextItem_c(const String_c &name);
			virtual ~ContextItem_c(void) {};			

			inline const String_c &GetName(void) const;			

			inline void Unlink();

			inline bool operator<(const ContextItem_c &rhs) const;

		private:	
			String_c strName;			
	};

	inline ContextItem_c::ContextItem_c(const String_c &name):
		strName(name)
	{
		//empty
	}	

	inline const String_c &ContextItem_c::GetName(void) const
	{
		return(strName);
	}	

	inline bool ContextItem_c::operator<(const ContextItem_c &rhs) const
	{
		return strName.compare(rhs.strName) < 0;
	}

	inline void ContextItem_c::Unlink()
	{
		unlink();
	}
}

#endif
