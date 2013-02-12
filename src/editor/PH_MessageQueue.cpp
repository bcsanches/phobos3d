#include "Editor/PH_MessageQueue.h"

Phobos::Editor::MessageQueue::MessageQueue()
{
	//empty
}

void Phobos::Editor::MessageQueue::Push(const std::string &msg)
{
	auto tempStr(msg);

	std::lock_guard<std::mutex> lock(m_clMutex);

	m_clQueue.push_back(std::move(tempStr));
}

Phobos::Editor::StringVector_t Phobos::Editor::MessageQueue::GetPendingMessages()
{
	StringVector_t ret;
	
	{	
		std::lock_guard<std::mutex> lock(m_clMutex);
		ret.swap(m_clQueue);
	}

	return ret;
}
