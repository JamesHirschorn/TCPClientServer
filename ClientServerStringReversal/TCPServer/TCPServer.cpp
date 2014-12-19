#include <iostream>
#include <string>

#include <ClientServerFramework/Server/TCPAsyncServer.hpp>
#include <ClientServerFramework/Server/TCPSession.hpp>
#include <ClientServerStringReversal/Shared/Data.h>
#include <ClientServerStringReversal/TCPServer/Action.h>

int main(int argc, char* argv[])
{
	using namespace std;
	using boost::asio::ip::tcp;
	using namespace ClientServer;

	try
	{
		boost::asio::io_service io_service;

		TCPAsyncServer<TCPSession<Action>> server(io_service, 12345);

		// Start up the server.
		io_service.run();
	}
	catch (exception& e)
	{
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}

}