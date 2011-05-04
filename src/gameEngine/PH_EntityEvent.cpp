#include "PH_EntityEvent.h"

namespace Phobos
{
	EntityEvent_c::EntityEvent_c(UInt_t type, Entity_c &activator, Entity_c &caller):		
		rclActivator(activator),
		rclCaller(caller),
		uType(type)
	{
		//empty
	}
}
