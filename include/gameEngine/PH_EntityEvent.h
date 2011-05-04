#ifndef PH_ENTITY_EVENT_H
#define PH_ENTITY_EVENT_H

#include <PH_Types.h>

namespace Phobos
{
	class Entity_c;

	class EntityEvent_c
	{
		public:
			EntityEvent_c(UInt_t type, Entity_c &activator, Entity_c &caller);

		private:
			Entity_c &rclActivator;
			Entity_c &rclCaller;

			UInt_t	uType;
	};
}

#endif
