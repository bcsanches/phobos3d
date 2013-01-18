#ifndef PH_EDITOR_REQUEST_H
#define PH_EDITOR_REQUEST_H

#include <rapidjson/document.h>
#include <JsonCreator/StringWriterFwd.h>

namespace Phobos
{
	namespace Editor
	{		
		class Request_c
		{
			public:
				Request_c(const rapidjson::Value &value);

				void Execute(JsonCreator::StringWriter &response);

			private:
				virtual void OnExecute(JsonCreator::StringWriter &response) = 0;
		};
	}
}

#endif
