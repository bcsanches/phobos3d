#include "Editor/PH_MessageQueue.h"

void Phobos::Editor::MessageQueue_c::Push(const std::string &msg)
{
	auto tempStr(msg);

	std::lock_guard<std::mutex> lock(clMutex);

	clQueue.push_back(std::move(tempStr));
}

Phobos::Editor::StringVector_t Phobos::Editor::MessageQueue_c::GetPendingMessages()
{
	StringVector_t ret;
	
	{	
		std::lock_guard<std::mutex> lock(clMutex);
		ret.swap(clQueue);
	}

	return ret;
}
