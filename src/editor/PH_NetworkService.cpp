#include "PH_NetworkService.h"

#include <boost/make_shared.hpp>
#include <websocketpp.hpp>

#include <PH_Kernel.h>
#include <PH_Memory.h>

namespace
{
	class ServerHandler_c: public websocketpp::server::handler
	{		 
		public:
			void validate(connection_ptr con); 
    
			// add new connection to the lobby
			void on_open(connection_ptr con);
        
			// someone disconnected from the lobby, remove them
			void on_close(connection_ptr con);
    
			void on_message(connection_ptr con, message_ptr msg);

		private:			
			std::string encode_message(std::string sender,std::string msg,bool escape = true);			
    
			void send_to_all(std::string data);    			
	};

	void ServerHandler_c::validate(connection_ptr con)
	{
	}

    void ServerHandler_c::on_open(connection_ptr con)
	{
	}
        
	
	void ServerHandler_c::on_close(connection_ptr con)
	{
	}
    
	void ServerHandler_c::on_message(connection_ptr con, message_ptr msg)
	{
	}
}

Phobos::Editor::NetworkService_c::NetworkService_c():
	clServerEndPoint(boost::make_shared<ServerHandler_c>())	
{
	//empty
}

Phobos::Editor::NetworkService_c::~NetworkService_c()
{
	Kernel_c &kernel = Kernel_c::GetInstance();

	kernel.LogMessage("[Phobos::Editor::NetworkService_c::~NetworkService_c] Stopping server.");
	clServerEndPoint.stop();

	kernel.LogMessage("[Phobos::Editor::NetworkService_c::~NetworkService_c] Server stopped, waiting thread.");

	clThread.join();

	kernel.LogMessage("[Phobos::Editor::NetworkService_c::~NetworkService_c] Thread joined.");
}

void Phobos::Editor::NetworkService_c::Start()
{	
	clThread.swap(boost::thread([&](){ clServerEndPoint.listen(2325); }));
}

