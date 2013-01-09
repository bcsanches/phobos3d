#include "Editor/PH_NetworkService.h"

#include <boost/make_shared.hpp>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <websocketpp.hpp>

#include <PH_Kernel.h>
#include <PH_Memory.h>

#include "Editor/PH_MessageQueue.h"

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

			Phobos::Editor::StringVector_t GetPendingMessages();

		private:			
			std::string encode_message(std::string sender,std::string msg,bool escape = true);			
    
			void send_to_all(std::string data);    		

			Phobos::Editor::MessageQueue_c clMessageQueue;
	};

	void ServerHandler_c::validate(connection_ptr con)
	{
		//nothing to validade
	}

    void ServerHandler_c::on_open(connection_ptr con)
	{
		rapidjson::StringBuffer stream;
		rapidjson::Writer<rapidjson::StringBuffer> writer(stream);

		writer.StartObject();

		writer.String("command").String("Log");
		writer.String("message").String("[Phobos::Editor::ServerHandler_c::on_open] Connection opened");

		writer.EndObject();		

		clMessageQueue.Push(stream.GetString());
	}
        
	
	void ServerHandler_c::on_close(connection_ptr con)
	{
		rapidjson::StringBuffer stream;
		rapidjson::Writer<rapidjson::StringBuffer> writer(stream);

		writer.StartObject();

		writer.String("command").String("Log");
		writer.String("message").String("[Phobos::Edittor::ServerHandler_c::on_open] Connection closed");

		writer.EndObject();		

		clMessageQueue.Push(stream.GetString());
	}
    
	void ServerHandler_c::on_message(connection_ptr con, message_ptr msg)
	{
		clMessageQueue.Push(msg->get_payload());
	}

	Phobos::Editor::StringVector_t ServerHandler_c::GetPendingMessages()
	{
		return clMessageQueue.GetPendingMessages();
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

Phobos::Editor::StringVector_t Phobos::Editor::NetworkService_c::GetPendingMessages()
{	
	return boost::static_pointer_cast<ServerHandler_c>(clServerEndPoint.get_handler())->GetPendingMessages();
}
