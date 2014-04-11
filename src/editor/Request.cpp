#include "Phobos/Editor/Request.h"

#include <JsonCreator/StringWriter.h>

Phobos::Editor::Request::Request(const rapidjson::Value &obj):
	m_uId(0),
	m_fNotification(true)
{	
	if(obj.HasMember("id"))
	{
		m_uId = obj["id"].GetInt();
		m_fNotification = false;
	}
}

void Phobos::Editor::Request::Execute(const rapidjson::Value &request, JsonCreator::StringWriter &response)
{
	auto parameters = request.HasMember("params") ? &request["params"] : nullptr;

	if(m_fNotification)
	{
		this->OnExecute(parameters, nullptr);
	}
	else
	{
		auto obj = JsonCreator::MakeObject(response);

		obj.AddStringValue("jsonrpc", "2.0");

		this->OnExecute(parameters, &obj);

		obj.AddIntValue("id", m_uId);
	}
}
