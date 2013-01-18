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
				LogRequest_c(const rapidjson::Value &value);

			protected:
				virtual void OnExecute(JsonCreator::StringWriter &response);

			private:
				String_c strMessage;
		};
	}
}


#endif
