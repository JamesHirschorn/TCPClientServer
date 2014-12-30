/** Server connection. */

#include <ClientServerFramework/Shared/Connection/Connection_base.hpp>
#include <ClientServerFramework/Shared/IO/Filter_base_factory.hpp>

namespace io {

	template<typename InternetProtocol>
	class ServerConnection : 
		public Connection_base<InternetProtocol>
	{
	public:
		typedef typename IO_base_type::acceptor_type acceptor_type;
		typedef typename IO_base_type::accept_handler accept_handler;
		typedef typename IO_base_type::initialize_handler initialize_handler;
		typedef typename IO_base_type::async_handshake_handler async_handshake_handler;

		ServerConnection(
			boost::asio::io_service& io_service,
			ssl_options const& SSL_options,
			bool compression) :
			Connection_base<internet_protocol>(
				IO_base_factory<internet_protocol>::create(io_service, SSL_options, IO_base_type::SERVER),
				Filter_base_factory::create(compression))
		{}

		/// addition server initialization, if any 
		void async_initialize(initialize_handler const& handler)
		{
			io().async_initialize(handler);
		}

		/// asynchronous handshake
		void async_handshake(async_handshake_handler const& handler)
		{
			io().async_handshake(boost::asio::ssl::stream_base::server, handler);
		}

		/// asychronous acceptor
		void async_accept(acceptor_type& acceptor, accept_handler const& handler)
		{
			io().async_accept(acceptor, handler);
		}
	};

}