#include <boost/asio/ip/tcp.hpp>

#include <ClientServerFramework/Server/Session.hpp>

namespace ClientServer {

	template<typename Action>
	class TCPSession :
		public Session < boost::asio::ip::tcp, Action >
	{
	public:
		TCPSession(socket_type const& socket) : Session(socket) {}
		TCPSession(socket_type&& socket) 
			: Session(socket)
		{
		}
	};

}