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

#ifndef PH_REF_COUNTER_H
#define PH_REF_COUNTER_H

#include <boost/detail/atomic_count.hpp>

#include "PH_Defs.h"
#include "PH_KernelAPI.h"
#include "PH_RefCounterFwd.h"

namespace boost
{
	inline void intrusive_ptr_add_ref(Phobos::RefCounter_c *p);
	inline void intrusive_ptr_release(Phobos::RefCounter_c *p);
};

namespace Phobos
{
	/**
		This is the base class for counting references, this was designed to be used with smart pointers and it is 
		ready to be used with boost::intrusive_ptr.

		This class was designed to be inherited for.
	*/
	class PH_KERNEL_API RefCounter_c
	{
		protected:
			RefCounter_c();
			RefCounter_c(const RefCounter_c &rhs);

			RefCounter_c &operator=(const RefCounter_c &rhs);

			virtual ~RefCounter_c() {};

		private:
			void AddRef();
			void ReleaseRef();

			friend inline void boost::intrusive_ptr_add_ref(RefCounter_c *);
			friend inline void boost::intrusive_ptr_release(RefCounter_c *);

		private:
			boost::detail::atomic_count tCount;

		private:
			void *operator new[] ( std::size_t sz );		
	};
}

namespace boost
{
	inline void intrusive_ptr_add_ref(Phobos::RefCounter_c *p)
	{
		p->AddRef();
	}

	inline void intrusive_ptr_release(Phobos::RefCounter_c *p)
	{
		p->ReleaseRef();
	}
}


#endif
