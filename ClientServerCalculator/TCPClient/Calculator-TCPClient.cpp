#include <iostream>
#include <string>

#include <ClientServerFramework/Client/TCPClient.hpp>
#include <ClientServerCalculator/Shared/Data.h>
#include <ClientServerCalculator/TCPClient/DataScraper.h>
#include <ClientServerCalculator/TCPClient/ClientActions.hpp>

int main(int argc, char* argv[])
{
	using namespace std;
	using namespace Client;

	/** This TCP client program is essentially just an instantiation of the 
		generic TCPClient class. Hence it is just a matter of plugging in the
		desired types:
			data - the type of the data sent by the client to the server.
			Response<string> - the type of the response that is returned by 
				the server to the client.
			DataScraper - the type of the scraper, which scrapes the data 
				from the input. 
			Strategy - what actions, if any, to perform on the data before 
				sending and to perform on the server response. 
	*/

	typedef data									data_type;
	typedef Server::Response<string>				response_type;
	typedef DataScraper								scraper_type;
	typedef ClientActions<data_type, response_type>	strategy_type;

	// the client type used internally
	typedef TCPClient<data_type, response_type, scraper_type, strategy_type>	
		client_type;

	try
	{
		string host, service, client_id, message;

		if (argc < 3 || argc > 4)
		{
			cerr << "Usage: " << argv[0] << " <host> <service/port> [client id]" << endl;
			exit(EXIT_FAILURE);
		}
		else
		{
			host = argv[1]; service = argv[2];
			if (argc == 4)
			{
				client_id = argv[3];
				message = " from Client Id: " + client_id;
			}
		}

		boost::asio::io_service io_service;

		strategy_type strategy(message);

		// Create the internal client, using cin as the input stream, 
		// and start it up.
		client_type::create(io_service, host, service, cin, strategy)->start();
	}
	catch (exception& e)
	{
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}

}