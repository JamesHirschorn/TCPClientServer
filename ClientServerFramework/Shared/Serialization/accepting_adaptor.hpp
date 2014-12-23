#include <boost/asio/basic_socket_acceptor.hpp>

namespace io {

	template<typename Connection>
	class accepting_adaptor :
		public Connection
	{
		typedef typename Connection::internet_protocol internet_protocol;
		typedef typename internet_protocol::endpoint endpoint_type;
	public:
		/// ctor
		/// IPv4 is hard-coded for now. 
		accepting_adaptor(boost::asio::io_service& io_service, short port) :
			Connection(io_service),
			acceptor_(io_service, endpoint_type(internet_protocol::v4(), port))
		{
		}

		typedef std::function<void(boost::system::error_code const&)> async_accept_handler_type;

		// Asynchronously establish a connection by trying each endpoint in a sequence.
		void async_accept(async_accept_handler_type const& handler)
		{
			acceptor_.async_accept(socket(), handler);
		}
	private:
		typedef typename internet_protocol::acceptor acceptor_type;
		acceptor_type acceptor_;
	};

}	// namespace io