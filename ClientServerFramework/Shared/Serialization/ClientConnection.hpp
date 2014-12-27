/** Client connection. */

#include <ClientServerFramework/Shared/Serialization/UnsecuredIO.hpp>
#include <ClientServerFramework/Shared/Serialization/Connection_base.hpp>

namespace io {

	template<typename InternetProtocol>
	class ClientConnection :
		public Connection_base<InternetProtocol>
	{
	public:
		ClientConnection(
			boost::asio::io_service& io_service, 
			ssl_options const& SSL_options) :
			Connection_base<internet_protocol>(
				IO_base_type::create(io_service, SSL_options))
		{}

		endpoint_iterator connect(
			endpoint_iterator begin,
			boost::system::error_code& ec)
		{
			return io().connect(begin, ec);
		}		
	};

}