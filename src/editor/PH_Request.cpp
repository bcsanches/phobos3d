#include "Editor/PH_Request.h"

Phobos::Editor::Request_c::Request_c(const json_spirit::mObject &obj)	
{

}

void Phobos::Editor::Request_c::Execute()
{
	this->OnExecute();
}