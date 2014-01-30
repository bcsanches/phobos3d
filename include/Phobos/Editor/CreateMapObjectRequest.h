#ifndef PH_EDITOR_CREATE_MAP_OBJECT_REQUEST_H
#define PH_EDITOR_CREATE_MAP_OBJECT_REQUEST_H

#include "Phobos/Editor/Request.h"

namespace Phobos
{
	namespace Editor
	{
		class CreateMapObjectRequest : public Request
		{
			public:
				CreateMapObjectRequest(const rapidjson::Value &value);

			private:
				virtual void OnExecute(const rapidjson::Value *parameters, JsonCreator::Object<JsonCreator::StringWriter> *response) override;
		};
	}
}

#endif
