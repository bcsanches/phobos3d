#ifndef PH_EDITOR_LOG_REQUEST_H
#define PH_EDITOR_LOG_REQUEST_H

#include "Phobos/Editor/Request.h"

#include <Phobos/String.h>

namespace Phobos
{
	namespace Editor
	{
		class LogRequest: public Request
		{
			public:
				LogRequest(const rapidjson::Value &value);

			protected:
				virtual void OnExecute(JsonCreator::StringWriter &response) override;

			private:
				String_t m_strMessage;
		};
	}
}


#endif
