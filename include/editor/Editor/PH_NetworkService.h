#ifndef PH_EDITOR_NETWORK_SERVICE_H
#define PH_EDITOR_NETWORK_SERVICE_H

#include <string>
#include <vector>

#include <websocketpp.hpp>

#include <boost/thread.hpp>

namespace Phobos
{
	namespace Editor
	{
		class NetworkService_c
		{
			public:
				NetworkService_c();
				~NetworkService_c();

				void Start();

				std::vector<std::string> GetPendingMessages();

			private:				
				websocketpp::server clServerEndPoint;
				boost::thread		clThread;
		};
	}
}

#endif
