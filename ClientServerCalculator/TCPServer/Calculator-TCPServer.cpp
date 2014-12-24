#include <ClientServerFramework/Server/TCPAsyncServer.hpp>
#include <ClientServerCalculator/TCPClient/client_data.hpp>
#include <ClientServerCalculator/TCPServer/server_data.hpp>
#include <ClientServerCalculator/TCPServer/ServerAction.hpp>

#include <iostream>
#include <string>

/// Hard-coded settings
/// Of course, these must be compatible with the client settings.
//io::ssl_mode const SSL_mode = io::SSLV23;	// (see SSL.hpp)
io::ssl_mode const SSL_mode = io::OFF;		// (no SSL, see SSL.hpp)
long const context_options =
	boost::asio::ssl::context::default_workarounds
	| boost::asio::ssl::context::no_sslv2
	| boost::asio::ssl::context::single_dh_use;
boost::asio::ssl::verify_mode const ssl_verify_mode = boost::asio::ssl::verify_peer;
std::string const install_dir = INSTALL_DIRECTORY;
std::string const ssl_subdir = "ssl";	// where to look for certificates
std::string const private_key_filename = "newcert.pem";
std::string const temporary_DH_filename = "dh512.pm";

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

		// Create the SSL options struct.
		string ssl_path = install_dir + '/' + ssl_subdir;
		io::ssl_options SSL_options = 
		{ 
			SSL_mode, 
			context_options,
			ssl_verify_mode, 
			ssl_path, 
			std::string(),
			private_key_filename, 
			temporary_DH_filename };

		server_type server(io_service, port, SSL_options, strategy);

		// Start up the server.
		io_service.run();
	}
	catch (exception& e)
	{
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}

}