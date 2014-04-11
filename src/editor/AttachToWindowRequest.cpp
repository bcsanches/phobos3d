#include "Phobos/Editor/AttachToWindowRequest.h"

#include "Phobos/Editor/RequestFactory.h"

#include <Phobos/OgreEngine/Render.h>

#include <JsonCreator/Object.h>
#include <JsonCreator/StringWriter.h>

namespace Phobos
{
	namespace Editor
	{
		PH_FULL_REQUEST_CREATOR("AttachToWindow", AttachToWindowRequest);

		AttachToWindowRequest::AttachToWindowRequest(const rapidjson::Value &value) :
			Request(value)
		{
			//empty
		}


		void AttachToWindowRequest::OnExecute(const rapidjson::Value *parameters, JsonCreator::Object<JsonCreator::StringWriter> *response)
		{
			void *ptr = reinterpret_cast<void *>(std::stoi((*parameters)["window"].GetString()));

			OgreEngine::Render::GetInstance().SetParentWindow(ptr);

			response->AddBool("result", true);
		}
	}
}

