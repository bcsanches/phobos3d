#ifndef PH_EDITOR_ATTACH_WINDOW_REQUEST_H
#define PH_EDITOR_ATTACH_WINDOW_REQUEST_H

#include "Phobos/Editor/Request.h"

namespace Phobos
{
	namespace Editor
	{
		class AttachToWindowRequest : public Request
		{
		public:
			AttachToWindowRequest(const rapidjson::Value &value);

		private:
			virtual void OnExecute(const rapidjson::Value *parameters, JsonCreator::Object<JsonCreator::StringWriter> *response) override;
		};
	}
}

#endif
