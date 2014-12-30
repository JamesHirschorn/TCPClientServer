/**
*	Specialization the Connector class for the TCP internet protocol.
*	See Connector.hpp for documentation.
*/

#include <ClientServerFramework/Client/Connector.hpp>

#include <boost/asio/ip/tcp.hpp>

#include <string>
#include <utility>

namespace Client {

	template<typename ClientData, typename Response>
	class TCPConnector : 
		public Connector<boost::asio::ip::tcp, ClientData, Response>
	{
	public:
		TCPConnector(
			boost::asio::io_service& io_service,
			io::ssl_options const& SSL_options, bool compression,
			std::string const& host, std::string const& service) :
			Connector(io_service, SSL_options, compression, host, service)
		{
		}
	};
}	// namespace Client