#include "Editor/PH_LogRequest.h"

#include "Editor/PH_RequestFactory.h"

#include <PH_Kernel.h>

namespace Phobos
{
	namespace Editor
	{
		PH_FULL_REQUEST_CREATOR("Log", LogRequest_c);
	}
}

Phobos::Editor::LogRequest_c::LogRequest_c(const rapidjson::Value &obj):
	Request_c(obj),
	strMessage(obj["message"].GetString())
{				
	//empty
}

void Phobos::Editor::LogRequest_c::OnExecute()
{
	Kernel_c::GetInstance().LogMessage(strMessage);
}
