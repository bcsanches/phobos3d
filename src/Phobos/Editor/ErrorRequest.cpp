#include "Phobos/Editor/ErrorRequest.h"

#include <Phobos/Log.h>

#include "Phobos/Editor/RequestFactory.h"

#include <JsonCreator/Object.h>
#include <JsonCreator/StringWriter.h>

#if 0
namespace Phobos
{
	namespace Editor
	{
		PH_FULL_REQUEST_CREATOR("Error", ErrorRequest);
	}
}
#endif

Phobos::Editor::ErrorRequest::ErrorRequest(const rapidjson::Value &value, const char *message, Errors error):
	Request(value),
	m_pszMessage(message),
	m_eError(error)
{
	//empty
}


void Phobos::Editor::ErrorRequest::OnExecute(JsonCreator::Object<JsonCreator::StringWriter> *response)
{	
	//Log the error
	LogMakeStream() << "[Phobos::Editor::ErrorRequest::OnExecute] JSON-RPC Error (id=" << this->GetId() << "): " << m_pszMessage;

	//No response required, no error report
	if(!response)
		return;

	auto errorObj = response->AddObject("error");

	errorObj.AddIntValue("code", m_eError);

	errorObj.AddStringValue("message", m_pszMessage);	
}
