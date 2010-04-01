#ifndef PH_CORE_MODULE_FWD_H
#define PH_CORE_MODULE_FWD_H

#include <boost/intrusive_ptr.hpp>

namespace Phobos
{
	class CoreModule_c;

	typedef boost::intrusive_ptr<CoreModule_c> CoreModulePtr_t;
}

#endif
