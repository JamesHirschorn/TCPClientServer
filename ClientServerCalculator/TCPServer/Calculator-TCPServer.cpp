#include <iostream>
#include <string>

#include <ClientServerFramework/Server/TCPAsyncServer.hpp>
#include <ClientServerFramework/Server/TCPSession.hpp>
#include <ClientServerCalculator/Shared/data.h>
#include <ClientServerCalculator/TCPServer/ServerAction.hpp>

int main(int argc, char* argv[])
{
	using namespace std;
	using boost::asio::ip::tcp;
	using namespace ClientServer;

	try
	{
		if (argc != 2)
		{
			cerr << "Usage: " << argv[0] << " <port>" << endl;
			exit(EXIT_FAILURE);
		}

		boost::asio::io_service io_service;

		typedef data											ClientData;
		typedef std::string										ServerData;
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