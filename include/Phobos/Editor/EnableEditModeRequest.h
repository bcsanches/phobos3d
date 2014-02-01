#ifndef PH_ENABLE_EDIT_MODE_REQUEST_H
#define PH_ENABLE_EDIT_MODE_REQUEST_H

#include "Phobos/Editor/Request.h"

namespace Phobos
{
	namespace Editor
	{
		class EnableEditModeRequest : public Request
		{
			public:
				EnableEditModeRequest(const rapidjson::Value &value);

			private:
				virtual void OnExecute(const rapidjson::Value *parameters, JsonCreator::Object<JsonCreator::StringWriter> *response) override;
		};
	}
}

#endif
