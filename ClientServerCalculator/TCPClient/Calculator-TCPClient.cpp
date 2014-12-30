#include <ClientServerFramework/Client/TCPClient.hpp>
#include <ClientServerCalculator/TCPServer/server_data.hpp>
#include <ClientServerCalculator/TCPClient/client_data.hpp>
#include <ClientServerCalculator/TCPClient/DataScraper.hpp>
#include <ClientServerCalculator/TCPClient/ClientActions.hpp>

#include <fstream>
#include <iostream>
#include <string>

/// Hard-coded settings
/// Of course, these must be compatible with the server settings.
io::ssl_mode const SSL_mode = io::SSLV23;	// (see SSL.hpp)
//io::ssl_mode const SSL_mode = io::SSLV3;// (see SSL.hpp)
//io::ssl_mode const SSL_mode = io::OFF;	// (no SSL, see SSL.hpp)
long const context_options = boost::asio::ssl::context::default_workarounds;
boost::asio::ssl::verify_mode const ssl_verify_mode = boost::asio::ssl::verify_peer;
std::string const install_dir = INSTALL_DIRECTORY;
std::string const ssl_subdir = "ssl";	// where to look for certificates
std::string const ca_filename = "ca_cert.pem";
bool use_compression = true;

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
			Strategy - what actions, if any, to perform:
				1. on the data before sending, and 
				2. on the server response. 

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

		// Create the SSL options struct.
		string ssl_path = install_dir + '/' + ssl_subdir;
		io::ssl_options SSL_options = { 
			SSL_mode, 
			context_options,
			ssl_verify_mode, 
			ssl_path, 
			ca_filename };

		// Create the internal client, using either cin or the given file as the input stream, 
		// and start it up.
		if (filename.empty())
			client_type::create(io_service, SSL_options, use_compression, client_id, host, service, cin, true, strategy)->start();
		else
		{
			ifstream is(filename);
			if (is)
				client_type::create(io_service, SSL_options, use_compression, client_id, host, service, is, false, strategy)->start();
			else
				throw std::runtime_error("Unable to open input file.");
		}
	}
	catch (exception& e)
	{
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
	catch (boost::system::error_code& e)
	{
		cerr << e.message() << endl;
		exit(EXIT_FAILURE);
	}
}