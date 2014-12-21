#include <iostream>
#include <string>

#include <ClientServerFramework/Server/TCPAsyncServer.hpp>
#include <ClientServerFramework/Server/TCPSession.hpp>
#include <ClientServerCalculator/TCPClient/client_data.hpp>
#include <ClientServerCalculator/TCPServer/server_data.hpp>
#include <ClientServerCalculator/TCPServer/ServerAction.hpp>

int main(int argc, char* argv[])
{
	using namespace std;
	using boost::asio::ip::tcp;
	using namespace Server;

	try
	{
		if (argc != 2)
		{
			cerr << "Usage: " << argv[0] << " <port>" << endl;
			exit(EXIT_FAILURE);
		}

		boost::asio::io_service io_service;

		typedef client_data										ClientData;
		typedef server_data										ServerData;
		typedef ServerAction<ClientData, ServerData>			Strategy;
		typedef TCPSession<Strategy, ClientData, ServerData>	Session;

		typedef TCPAsyncServer<Session, Strategy>				server_type;

		short port = std::atoi(argv[1]);

		Strategy strategy;

		server_type server(io_service, port, strategy);

		// Start up the server.
		io_service.run();
	}
	catch (exception& e)
	{
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}

}