#ifndef PH_EDITOR_LOG_REQUEST_H
#define PH_EDITOR_LOG_REQUEST_H

#include "Editor/PH_Request.h"

#include <PH_String.h>

namespace Phobos
{
	namespace Editor
	{
		class LogRequest_c: public Request_c
		{
			public:
				LogRequest_c(const json_spirit::mObject &value);

			protected:
				virtual void OnExecute();

			private:
				String_c strMessage;
		};
	}
}


#endif
