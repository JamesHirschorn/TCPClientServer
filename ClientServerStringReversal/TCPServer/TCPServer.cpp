#include <iostream>
#include <string>

#include <ClientServerFramework/Server/TCPAsyncServer.hpp>
#include <ClientServerFramework/Server/TCPSession.hpp>
#include <ClientServerStringReversal/Shared/data.h>
#include <ClientServerStringReversal/TCPServer/Action.hpp>

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

		typedef data ClientData;
		typedef std::string ServerData;
		typedef Action<ServerData> Strategy;
		TCPAsyncServer<TCPSession<Strategy, ClientData, ServerData>> server(io_service, std::atoi(argv[1]));

		// Start up the server.
		io_service.run();
	}
	catch (exception& e)
	{
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}

}