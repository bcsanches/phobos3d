#ifndef PH_NETWORK_SERVICE_H
#define PH_NETWORK_SERVICE_H

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

			private:				
				websocketpp::server clServerEndPoint;
				boost::thread		clThread;
		};
	}
}

#endif
