#ifndef PH_EDITOR_NETWORK_SERVICE_H
#define PH_EDITOR_NETWORK_SERVICE_H

#include <vector>

#include <websocketpp.hpp>

#include <boost/thread.hpp>

namespace Phobos
{
	namespace Editor
	{
		class NetworkService
		{
			public:
				NetworkService();
				~NetworkService();

				void Start();

				void SendMessage(const std::string &msg);

				std::vector<std::string> GetPendingMessages();

			private:				
				websocketpp::server m_clServerEndPoint;
				boost::thread		m_clThread;
		};
	}
}

#endif
