#include <utility>

#include <boost/asio/ip/tcp.hpp>

#include <ClientServerFramework/Client/Connector.hpp>

namespace ClientServer {

	template<typename Data>
	class TCPConnector : public Connector
	{
	public:
		TCPConnector(std::string const& host, std::string const& service)
			: Connector(host, string), 
			// NO: preferable to use a move-ctor here
			resolver(io_service)
		{
		}
	private:
		tcp::resolver resolver_;
	};
}