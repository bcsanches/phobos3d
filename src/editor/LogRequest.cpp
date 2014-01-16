#include "Phobos/Editor/LogRequest.h"

#include "Phobos/Editor/RequestFactory.h"

#include <Phobos/Log.h>
#include <Phobos/Error.h>

#include <JsonCreator/StringWriter.h>

namespace Phobos
{
	namespace Editor
	{
		PH_FULL_REQUEST_CREATOR("Log", LogRequest);
	}
}

Phobos::Editor::LogRequest::LogRequest(const rapidjson::Value &obj):
	Request(obj),
	m_strMessage(obj["params"].GetString())
{				
	//empty
}

void Phobos::Editor::LogRequest::OnExecute(const rapidjson::Value *parameters, JsonCreator::Object<JsonCreator::StringWriter> *response)
{
	PH_ASSERT(response == nullptr);

	LogMessage(m_strMessage);
}
