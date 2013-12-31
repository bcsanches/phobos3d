#include "Phobos/Editor/NetworkService.h"

#include <boost/make_shared.hpp>
#include <JsonCreator/Object.h>
#include <JsonCreator/StringWriter.h>
#include <websocketpp.hpp>

#include <Phobos/Log.h>
#include <Phobos/Memory.h>

#include "Phobos/Editor/MessageQueue.h"

namespace
{
	class ServerHandler: public websocketpp::server::handler
	{		 
		public:
			ServerHandler();

			void validate(connection_ptr con); 
    
			// add new connection to the lobby
			void on_open(connection_ptr con);
        
			// someone disconnected from the lobby, remove them
			void on_close(connection_ptr con);
    
			void on_message(connection_ptr con, message_ptr msg);

			Phobos::Editor::StringVector_t GetPendingMessages();

			void SendMessage(const std::string &msg);

		private:			
			std::string encode_message(std::string sender,std::string msg,bool escape = true);			
    
			void send_to_all(std::string data);    		

			Phobos::Editor::MessageQueue m_clMessageQueue;

			connection_ptr m_spConnection;
	};

	ServerHandler::ServerHandler()
	{
		//empty
	}

	void ServerHandler::validate(connection_ptr con)
	{
		//nothing to validade
	}

    void ServerHandler::on_open(connection_ptr con)
	{
		m_spConnection = con;

		JsonCreator::StringWriter writer;

		{
			auto obj = JsonCreator::MakeObject(writer);

			obj.AddStringValue("jsonrpc", "2.0");
			obj.AddStringValue("method", "Log");
			obj.AddStringValue("params", "[Phobos::Editor::ServerHandler::on_open] Connection opened");			
		}
		
		m_clMessageQueue.Push(writer.GetString());
	}
        
	
	void ServerHandler::on_close(connection_ptr con)
	{
		JsonCreator::StringWriter writer;

		{
			auto obj = JsonCreator::MakeObject(writer);

			obj.AddStringValue("jsonrpc", "2.0");
			obj.AddStringValue("method", "Log");
			obj.AddStringValue("params", "[Phobos::Editor::ServerHandler::on_open] Connection closed");
		}

		m_clMessageQueue.Push(writer.GetString());

		m_spConnection.reset();
	}
    
	void ServerHandler::on_message(connection_ptr con, message_ptr msg)
	{
		m_clMessageQueue.Push(msg->get_payload());
	}

	Phobos::Editor::StringVector_t ServerHandler::GetPendingMessages()
	{
		return m_clMessageQueue.GetPendingMessages();
	}

	void ServerHandler::SendMessage(const std::string &msg)
	{
		m_spConnection->send(msg);
	}
}

Phobos::Editor::NetworkService::NetworkService():
	m_clServerEndPoint(boost::make_shared<ServerHandler>())	
{
	//empty
}

Phobos::Editor::NetworkService::~NetworkService()
{
	LogMessage("[Phobos::Editor::NetworkService::~NetworkService] Stopping server.");
	m_clServerEndPoint.stop();

	LogMessage("[Phobos::Editor::NetworkService::~NetworkService] Server stopped, waiting thread.");

	m_clThread.join();

	LogMessage("[Phobos::Editor::NetworkService::~NetworkService] Thread joined.");
}

void Phobos::Editor::NetworkService::Start()
{	
	m_clThread.swap(boost::thread([&](){ m_clServerEndPoint.listen(2325); }));
}

Phobos::Editor::StringVector_t Phobos::Editor::NetworkService::GetPendingMessages()
{	
	return boost::static_pointer_cast<ServerHandler>(m_clServerEndPoint.get_handler())->GetPendingMessages();
}

void Phobos::Editor::NetworkService::SendMessage(const std::string &msg)
{
	boost::static_pointer_cast<ServerHandler>(m_clServerEndPoint.get_handler())->SendMessage(msg);
}
