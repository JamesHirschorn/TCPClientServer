#include <iostream>
#include <string>

#include <ClientServerFramework/Client/TCPConnector.hpp>

// A trivial data type.
struct Data
{
	std::string message;
};

int main(int argc, char* argv[])
{
	using namespace std;
	using namespace ClientServer;

	try
	{
		TCPConnector<Data> conn("localhost", "12345");

		if (!conn.open())
		{
			cerr << "Unable to open connection." << endl;
			exit(EXIT_FAILURE);
		}

		Data d;
		d.message = "Testing...";

		cout << "Sending message to server..." << endl;

		auto response = conn.send(d);

		cout << "Received response from server: " << response.message << endl;
	}
	catch (exception& e)
	{
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}

}