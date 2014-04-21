#include "Phobos/Editor/CreateMapObjectRequest.h"

#include "Phobos/Editor/EditObject.h"
#include "Phobos/Editor/EditorModule.h"
#include "Phobos/Editor/RequestFactory.h"

#include <JsonCreator/Object.h>
#include <JsonCreator/StringWriter.h>

#include <Phobos/Game/MapDefs.h>

namespace Phobos
{
	namespace Editor
	{
		PH_FULL_REQUEST_CREATOR("CreateMapObject", CreateMapObjectRequest);
	}
}

Phobos::Editor::CreateMapObjectRequest::CreateMapObjectRequest(const rapidjson::Value &value) :
	Request(value)
{
	//empty
}


void Phobos::Editor::CreateMapObjectRequest::OnExecute(const rapidjson::Value *parameters, JsonCreator::Object<JsonCreator::StringWriter> *response)
{				
	auto parentId = (*parameters)["parentId"].GetInt();
	auto &asset = (*parameters)["asset"];
	auto name = asset["name"].GetString();
	auto typeName = asset["type"].GetString();

	auto type = Game::StringToMapObjectType(typeName);

	auto &editObject = EditorModule::GetInstance().CreateMapObject(parentId, name, type);

	auto object = response->AddObject("result");

	object.AddStringValue("name", editObject.GetName().c_str());
	object.AddIntValue("id", editObject.GetId());
}
