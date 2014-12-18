#include <string>
#include <utility>

#include <boost/asio/ip/tcp.hpp>

#include <ClientServerFramework/Client/Connector.hpp>

namespace ClientServer {

	template<typename Data>
	class TCPConnector : 
		public Connector<boost::asio::ip::tcp, Data>
	{
	public:
		TCPConnector(std::string const& host, std::string const& service)
			: Connector(host, service)
		{
		}
	};
}