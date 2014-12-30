/** Client connection. */

#include <ClientServerFramework/Shared/Connection/Connection_base.hpp>
#include <ClientServerFramework/Shared/IO/Filter_base_factory.hpp>

namespace io {

	template<typename InternetProtocol>
	class ClientConnection :
		public Connection_base<InternetProtocol>
	{
	public:
		ClientConnection(
			boost::asio::io_service& io_service, 
			ssl_options const& SSL_options,
			bool compression) :
			Connection_base<internet_protocol>(
				IO_base_factory<internet_protocol>::create(io_service, SSL_options, IO_base_type::CLIENT),
				Filter_base_factory::create(compression))
		{}

		endpoint_iterator connect(
			endpoint_iterator begin,
			boost::system::error_code& ec)
		{
			return io().connect(begin, IO_base_type::CLIENT, ec);
		}		

		/// blocking handshake
		boost::system::error_code handshake(boost::system::error_code& ec)
		{
			return io().handshake(boost::asio::ssl::stream_base::client, ec);
		}
	};

}