#ifndef PH_EDITOR_MESSAGE_QUEUE_H
#define PH_EDITOR_MESSAGE_QUEUE_H

#include <mutex>
#include <string>
#include <vector>

#include <boost/noncopyable.hpp>

namespace Phobos
{
	namespace Editor
	{
		typedef std::vector<std::string> StringVector_t;

		class MessageQueue_c: boost::noncopyable
		{
			public:				
				void Push(const std::string &msg);

				StringVector_t GetPendingMessages();

			private:
				std::mutex		clMutex;

				StringVector_t	clQueue;
		};
	}
}

#endif
