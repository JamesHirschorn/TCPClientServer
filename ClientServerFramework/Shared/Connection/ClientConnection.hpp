/** Client connection. */

#include <ClientServerFramework/Shared/Connection/Connection_base.hpp>
#include <ClientServerFramework/Shared/IO/Filter_base_factory.hpp>

namespace io {

	template<typename InternetProtocol>
	class ClientConnection :
		public Connection_base<InternetProtocol>
	{
		typedef Connection_base<InternetProtocol> base_type;
	public:
		INHERIT(internet_protocol);
		INHERIT(IO_base_type);
		INHERIT(endpoint_iterator);
	  
		ClientConnection(
			boost::asio::io_service& io_service, 
			ssl_options const& SSL_options,
			bool compression) :
			Connection_base<internet_protocol>(
				IO_base_factory<internet_protocol>::create(io_service, SSL_options, CLIENT),
				Filter_base_factory::create(compression))
		{}

		endpoint_iterator connect(
			endpoint_iterator begin,
			boost::system::error_code& ec)
		{
			return base_type::io().connect(begin, CLIENT, ec);
		}		

		/// blocking handshake
		boost::system::error_code handshake(boost::system::error_code& ec)
		{
			return base_type::io().handshake(boost::asio::ssl::stream_base::client, ec);
		}
	};

}