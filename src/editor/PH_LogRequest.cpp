#include "Editor/PH_LogRequest.h"

#include "Editor/PH_RequestFactory.h"

#include <Phobos/Log.h>

namespace Phobos
{
	namespace Editor
	{
		PH_FULL_REQUEST_CREATOR("Log", LogRequest);
	}
}

Phobos::Editor::LogRequest::LogRequest(const rapidjson::Value &obj):
	Request(obj),
	m_strMessage(obj["message"].GetString())
{				
	//empty
}

void Phobos::Editor::LogRequest::OnExecute(JsonCreator::StringWriter &)
{
	LogMessage(m_strMessage);
}
