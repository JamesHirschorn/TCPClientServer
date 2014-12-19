#include <boost/asio/ip/tcp.hpp>

#include <ClientServerFramework/Server/Session.hpp>

namespace ClientServer {

	template<typename Strategy, typename ClientData, typename ServerData>
	class TCPSession :
		public Session<boost::asio::ip::tcp, Strategy, ClientData, ServerData>
	{
	public:
		explicit TCPSession(socket_type const& socket) : 
			Session(socket) {}
	};

}