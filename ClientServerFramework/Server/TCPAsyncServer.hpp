/**
*	Specialization the AsyncServer class for the TCP internet protocol.
*	See AsyncServer.hpp for documentation.
*/

#include <ClientServerFramework/Server/AsyncServer.hpp>
#include <ClientServerFramework/Server/TCPSession.hpp>

#include <boost/asio/io_service.hpp>

namespace Server {

	template<typename ClientData, typename ServerData, typename Strategy>
	class TCPAsyncServer :
		public AsyncServer<TCPSession<Strategy, ClientData, ServerData>, Strategy>
	{
	public:
		TCPAsyncServer(
			boost::asio::io_service& io_service, short port, 
			io::ssl_options const& SSL_options, 
			Strategy const& strategy) :
			AsyncServer(io_service, port, SSL_options, strategy)
		{
		}
	};

}	// namespace Server