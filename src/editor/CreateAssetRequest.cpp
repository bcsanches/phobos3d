#include "Phobos/Editor/CreateAssetRequest.h"

#include "Phobos/Editor/RequestFactory.h"

#include <JsonCreator/Object.h>
#include <JsonCreator/StringWriter.h>

namespace Phobos
{
	namespace Editor
	{
		PH_FULL_REQUEST_CREATOR("CreateAsset", CreateAssetRequest);
	}
}

Phobos::Editor::CreateAssetRequest::CreateAssetRequest(const rapidjson::Value &value):
	Request(value)
{
	//empty
}


void Phobos::Editor::CreateAssetRequest::OnExecute(const rapidjson::Value *parameters, JsonCreator::Object<JsonCreator::StringWriter> *response)
{			
	auto assetArray = response->AddArray("result");

	auto name = (*parameters)["name"].GetString();
	auto type = (*parameters)["type"].GetString();
}
