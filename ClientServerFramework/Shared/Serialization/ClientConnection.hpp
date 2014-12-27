/** Client connection. */

#include <ClientServerFramework/Shared/Serialization/UnsecuredIO.hpp>
#include <ClientServerFramework/Shared/Serialization/Connection_base.hpp>
#include <ClientServerFramework/Shared/Serialization/IO_base_factory.hpp>

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
				IO_base_factory<internet_protocol>::create(io_service, SSL_options, IO_base_type::CLIENT))
		{}

		endpoint_iterator connect(
			endpoint_iterator begin,
			boost::system::error_code& ec)
		{
			return io().connect(begin, ec);
		}		

		/// blocking handshake
		boost::system::error_code handshake(boost::system::error_code& ec)
		{
			return io().handshake(boost::asio::ssl::stream_base::client, ec);
		}
	};

}