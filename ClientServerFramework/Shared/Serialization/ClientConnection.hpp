/** Basic Client connection, with no SSL (or compression). */

#include <ClientServerFramework/Shared/Serialization/Connection.hpp>

namespace io {

	template<typename InternetProtocol>
	class ClientConnection :
		public Connection < InternetProtocol >
	{
	public:
		ClientConnection(boost::asio::io_service& io_service) :
			Connection<InternetProtocol>(io_service)
		{}
	};

}