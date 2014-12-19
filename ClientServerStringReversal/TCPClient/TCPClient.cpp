#include <iostream>
#include <string>

#include <ClientServerFramework/Client/TCPConnector.hpp>
#include <ClientServerStringReversal/Shared/Data.h>

int main(int argc, char* argv[])
{
	using namespace std;
	using namespace ClientServer;

	try
	{
		if (argc != 3)
		{
			cerr << "Usage: " << argv[0] << " <host> <port>" << endl;
			exit(EXIT_FAILURE);
		}

		boost::asio::io_service io_service;

		TCPConnector<data, Response<std::string>> conn(io_service, argv[1], argv[2]);

		if (!conn.open())
		{
			cerr << "Unable to open connection." << endl;
			exit(EXIT_FAILURE);
		}

		data d = { 3, 0.5, "Testing..." };

		cout << "Sending message to server..." << endl;

		auto response = conn.send(d);

		cout << "Received response from server: " << response.data << endl;
	}
	catch (exception& e)
	{
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}

}