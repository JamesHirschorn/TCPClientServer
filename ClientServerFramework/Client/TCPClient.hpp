/**
*	Specialization the Client class for the TCP internet protocol.
*	See Client.hpp for documentation.
*/

#include <ClientServerFramework/Client/Client.hpp>
#include <ClientServerFramework/Client/TCPConnector.hpp>

namespace Client {

	template<typename ClientData, typename Response, typename Scraper, typename Strategy>
	class TCPClient :
		public Client < TCPConnector<ClientData, Response>, Scraper, Strategy >
	{
	};

}	// namespace Client