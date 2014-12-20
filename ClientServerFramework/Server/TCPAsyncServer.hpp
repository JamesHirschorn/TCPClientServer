/**
*	Specialization the AsyncServer class for the TCP internet protocol.
*	See AsyncServer.hpp for documentation.
*/

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <ClientServerFramework/Server/AsyncServer.hpp>

namespace Server {

	template<typename SessionType, typename Strategy>
	class TCPAsyncServer :
		public AsyncServer<boost::asio::ip::tcp, SessionType, Strategy>
	{
	public:
		TCPAsyncServer(boost::asio::io_service& io_service, short port, Strategy const& strategy) :
			AsyncServer(io_service, port, strategy)
		{
		}
	};

}	// namespace Server