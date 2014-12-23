/** 
 *	Specialization the Session class for the TCP internet protocol. 
 *	See Session.hpp for documentation.
 */

#include <boost/asio/ip/tcp.hpp>

#include <ClientServerFramework/Server/Session.hpp>

namespace Server {

	template<typename Strategy, typename ClientData, typename ServerData>
	class TCPSession :
		public Session<boost::asio::ip::tcp, Strategy, ClientData, ServerData>
	{
	};

}	// namespace Server