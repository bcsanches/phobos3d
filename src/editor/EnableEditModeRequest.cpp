#include "Phobos/Editor/EnableEditModeRequest.h"

#include "Phobos/Editor/EditorModule.h"
#include "Phobos/Editor/RequestFactory.h"

namespace Phobos
{
	namespace Editor
	{
		PH_FULL_REQUEST_CREATOR("EnableEditMode", EnableEditModeRequest);

		EnableEditModeRequest::EnableEditModeRequest(const rapidjson::Value &value):
			Request(value)
		{
			//empty
		}

		void EnableEditModeRequest::OnExecute(const rapidjson::Value *parameters, JsonCreator::Object<JsonCreator::StringWriter> *response)
		{
			EditorModule::GetInstance().EnableEditMode();
		}
	}
}
