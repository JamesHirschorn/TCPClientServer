/** Server connection. */

#include <ClientServerFramework/Shared/Serialization/UnsecuredIO.hpp>
#include <ClientServerFramework/Shared/Serialization/Connection_base.hpp>

namespace io {

	template<typename InternetProtocol>
	class ServerConnection : 
		public Connection_base<InternetProtocol>
	{
	public:
		typedef typename IO_base_type::acceptor_type acceptor_type;
		typedef typename IO_base_type::initialize_handler initialize_handler;
		typedef typename IO_base_type::accept_handler accept_handler;

		ServerConnection(
			boost::asio::io_service& io_service,
			ssl_options const& SSL_options) :
			Connection_base<internet_protocol>(
				IO_base_type::create(io_service, SSL_options))
		{}

		/// addition server initialization, if any 
		void async_initialize(initialize_handler const& handler)
		{
			io().async_initialize(handler);
		}

		/// asychronous acceptor
		void async_accept(acceptor_type& acceptor, accept_handler const& handler)
		{
			io().async_accept(acceptor, handler);
		}
	};

}