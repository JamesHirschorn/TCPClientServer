#include <fstream>
#include <iostream>
#include <string>

#include <ClientServerFramework/Client/TCPClient.hpp>
#include <ClientServerCalculator/TCPServer/server_data.hpp>
#include <ClientServerCalculator/TCPClient/client_data.hpp>
#include <ClientServerCalculator/TCPClient/DataScraper.hpp>
#include <ClientServerCalculator/TCPClient/ClientActions.hpp>

int main(int argc, char* argv[])
{
	using namespace std;
	using namespace Client;

	/** This TCP client program is essentially just an instantiation of the 
		generic TCPClient class. Hence it is just a matter of plugging in the
		desired types:
			client_data - the type of the data sent by the client to the server.
			Response<server_data> - the type of the response that is returned by 
				the server to the client.
			DataScraper - the type of the scraper, which scrapes the data 
				from the input. 
			Strategy - what actions, if any, to perform on the data before 
				sending and to perform on the server response. 

		The data source the specified input file if given, or cin otherwise.
	*/

	typedef client_data									request_type;
	typedef Server::Response<server_data>				response_type;
	typedef DataScraper									scraper_type;
	typedef ClientActions<request_type, response_type>	strategy_type;

	// the client type used internally
	typedef TCPClient<request_type, response_type, scraper_type, strategy_type>	
		client_type;

	try
	{
		string host, service, client_id, filename;

		if (argc != 4 && argc != 5)
		{
			cerr << "Usage: " << argv[0] << " <client id> <host> <service/port> [<input filename>]" << endl;
			exit(EXIT_FAILURE);
		}
			
		client_id = argv[1];  host = argv[2]; service = argv[3];

		if (argc > 4)
			filename = argv[4];

		boost::asio::io_service io_service;

		strategy_type strategy;

		// Create the internal client, using cin as the input stream, 
		// and start it up.
		if (filename.empty())
			client_type::create(io_service, client_id, host, service, cin, true, strategy)->start();
		else
		{
			ifstream is(filename);
			client_type::create(io_service, client_id, host, service, is, false, strategy)->start();
		}
	}
	catch (exception& e)
	{
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}