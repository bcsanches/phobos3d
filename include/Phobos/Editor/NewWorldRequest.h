#ifndef PH_EDITOR_NEW_WORLD_REQUEST_H
#define PH_EDITOR_NEW_WORLD_REQUEST_H

#include "Phobos/Editor/Request.h"

namespace Phobos
{
	namespace Editor
	{
		class NewWorldRequest: public Request
		{
			public:
				NewWorldRequest(const rapidjson::Value &value);

			private:
				virtual void OnExecute(JsonCreator::Object<JsonCreator::StringWriter> *response) override;
		};
	}
}

#endif
