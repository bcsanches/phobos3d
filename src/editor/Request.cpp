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

void Phobos::Editor::Request::Execute(JsonCreator::StringWriter &response)
{
	if(m_fNotification)
	{
		this->OnExecute(nullptr);
	}
	else
	{
		auto obj = JsonCreator::MakeObject(response);

		obj.AddStringValue("jsonrpc", "2.0");

		this->OnExecute(&obj);

		obj.AddIntValue("id", m_uId);
	}
}
