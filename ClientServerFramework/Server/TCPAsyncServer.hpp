/**
*	Specialization the AsyncServer class for the TCP internet protocol.
*	See AsyncServer.hpp for documentation.
*/

#include <ClientServerFramework/Server/AsyncServer.hpp>
#include <ClientServerFramework/Server/TCPSession.hpp>

#include <boost/asio/io_service.hpp>

namespace Server {

	template<typename SessionType, typename Strategy>
	class TCPAsyncServer :
		public AsyncServer<SessionType, Strategy>
	{
	public:
		TCPAsyncServer(boost::asio::io_service& io_service, short port, Strategy const& strategy) :
			AsyncServer(io_service, port, strategy)
		{
		}
	};

}	// namespace Server