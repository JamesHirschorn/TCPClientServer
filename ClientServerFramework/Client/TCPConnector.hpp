/**
*	Specialization the Connector class for the TCP internet protocol.
*	See Connector.hpp for documentation.
*/

#include <string>
#include <utility>

#include <boost/asio/ip/tcp.hpp>

#include <ClientServerFramework/Client/Connector.hpp>

namespace Client {

	template<typename ClientData, typename Response>
	class TCPConnector : 
		public Connector<boost::asio::ip::tcp, ClientData, Response>
	{
	public:
		TCPConnector(boost::asio::io_service& io_service, std::string const& host, std::string const& service)
			: Connector(io_service, host, service)
		{
		}
	};
}	// namespace Client