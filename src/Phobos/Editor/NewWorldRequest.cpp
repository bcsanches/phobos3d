#include "Phobos/Editor/NewWorldRequest.h"

#include "Phobos/Editor/RequestFactory.h"

#include <Phobos/Register/Hive.h>
#include <Phobos/Register/Manager.h>
#include <Phobos/Register/Table.h>

#include <Phobos/Game/Things/Keys.h>

#include <OgreResourceGroupManager.h>

#include <JsonCreator/Object.h>
#include <JsonCreator/StringWriter.h>

namespace Phobos
{
	namespace Editor
	{
		PH_FULL_REQUEST_CREATOR("NewWorld", NewWorldRequest);
	}
}

Phobos::Editor::NewWorldRequest::NewWorldRequest(const rapidjson::Value &value):
	Request(value)
{
	//empty
}


void Phobos::Editor::NewWorldRequest::OnExecute(JsonCreator::Object<JsonCreator::StringWriter> *response)
{		
	response->AddStringValue("result", "Ok");
}