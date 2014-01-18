#include "Phobos/Editor/NewWorldRequest.h"

#include "Phobos/Editor/RequestFactory.h"

#include <Phobos/Game/WorldManager.h>

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


void Phobos::Editor::NewWorldRequest::OnExecute(const rapidjson::Value *parameters, JsonCreator::Object<JsonCreator::StringWriter> *response)
{			
	Game::WorldManager::GetInstance().LoadBlankMap("NewLevel");

	//auto result = response->AddObject("result");
}
