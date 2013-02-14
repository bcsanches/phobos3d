#include "Editor/PH_Request.h"

Phobos::Editor::Request::Request(const rapidjson::Value &obj)	
{

}

void Phobos::Editor::Request::Execute(JsonCreator::StringWriter &response)
{
	this->OnExecute(response);
}