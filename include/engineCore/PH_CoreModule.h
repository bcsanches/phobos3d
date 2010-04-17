#ifndef PH_CORE_MODULE_H
#define PH_CORE_MODULE_H

#include <PH_Node.h>

#include "PH_CoreModuleFwd.h"

namespace Phobos
{
	class CoreModule_c: public Node_c
	{
		protected:
			explicit CoreModule_c(const String_c &name);
			explicit CoreModule_c(const Char_t *name);
			~CoreModule_c();
	};
}

#endif
