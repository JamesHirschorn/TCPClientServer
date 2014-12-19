/**
*	Specialization the AsyncServer class for the TCP internet protocol.
*	See AsyncServer.hpp for documentation.
*/

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <ClientServerFramework/Server/AsyncServer.hpp>

namespace ClientServer {

	template<typename SessionType>
	class TCPAsyncServer :
		public AsyncServer<boost::asio::ip::tcp, SessionType>
	{
	public:
		TCPAsyncServer(boost::asio::io_service& io_service, short port) :
			AsyncServer(io_service, port)
		{
		}
	};

}