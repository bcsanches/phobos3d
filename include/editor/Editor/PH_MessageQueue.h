#ifndef PH_EDITOR_MESSAGE_QUEUE_H
#define PH_EDITOR_MESSAGE_QUEUE_H

#include <mutex>
#include <string>
#include <vector>

#include <Phobos/DisableCopy.h>

namespace Phobos
{
	namespace Editor
	{
		typedef std::vector<std::string> StringVector_t;

		class MessageQueue
		{
			PH_DISABLE_COPY(MessageQueue);

			public:		
				MessageQueue();

				void Push(const std::string &msg);

				StringVector_t GetPendingMessages();

			private:
				std::mutex		m_clMutex;

				StringVector_t	m_clQueue;
		};
	}
}

#endif
