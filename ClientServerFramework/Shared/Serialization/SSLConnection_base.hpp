/**
 *	Abstract base class for SSLConnection, to be subclassed 
 *	for the client and server separately.
 */

#include <ClientServerFramework/Shared/SSL/SSL.hpp>
#include <ClientServerFramework/Shared/Serialization/Connection_base.hpp>

#include <boost/asio/ssl/context.hpp>
#include <boost/asio/ssl/stream.hpp>

#include <iostream>

namespace io {

	template<typename InternetProtocol>
	class SSLConnection_base :
		public Connection_base<
			InternetProtocol, 
			typename boost::asio::ssl::stream<typename InternetProtocol::socket>::lowest_layer_type>
	{
		typedef InternetProtocol internet_protocol;
		typedef boost::asio::ssl::stream<typename internet_protocol::socket> socket_type;
		typedef boost::asio::ssl::context context_type;
	public:
		/// ctor
		SSLConnection_base(
			boost::asio::io_service& io_service, 
			ssl_options const& options) :
			context_(options.get_method()),
			socket_(io_service, context_)
		{
			set_socket();
			set_context();
		}

		endpoint_iterator_type connect(
			endpoint_iterator_type begin,
			boost::system::error_code& ec)
		{
			endpoint_iterator_type endpoint = 
				boost::asio::connect(socket_.lowest_layer(), begin, ec);

			if (!ec)
			{
				// Perform SSL handshake immediately after openning the connection.
				handshake(ec);
				if (ec)
				{
					std::cerr << "SSL Handshake failed with error message: " << ec.message() << std::endl;
					throw ec;
				}
			}

			return endpoint;
		}

		// blocking handshake
		virtual boost::system::error_code handshake(boost::system::error_code & ec) = 0;

		/// dtor
		virtual ~SSLConnection_base() = 0;
	protected:
		/// socket inspector
		socket_type& socket()
		{
			return socket_;
		}
		lowest_layer_type& lowest_layer_socket()
		{
			return socket_.lowest_layer();
		}
		/// context inspector
		context_type& context()
		{
			return context_;
		}
	private:
		/// set up the socket (does nothing by default)
		virtual void set_socket() {}
		/// set up the context (does nothing by default)
		virtual void set_context() {};

		context_type context_;

		/// The underlying socket.
		socket_type socket_;

		/* implementation of abstract methods */

		void async_write_impl(
			std::vector<boost::asio::const_buffer> const& buffers,
			async_handler_type const& handler)
		{
			boost::asio::async_write(socket_, buffers, handler);
		}

		void async_read_impl(
			input_header_type& input,
			async_handler_type const& handler)
		{
			boost::asio::async_read(socket_, boost::asio::buffer(input), handler);
		}

		void async_read_impl(
			std::vector<char>& input,
			async_handler_type const& handler)
		{
			boost::asio::async_read(socket_, boost::asio::buffer(input), handler);
		}

		std::size_t write_impl(
			std::vector<boost::asio::const_buffer> const& buffers,
			boost::system::error_code& ec)
		{
			return boost::asio::write(socket_, buffers, boost::asio::transfer_all(), ec);
		}

		std::size_t read_impl(
			input_header_type& input,
			boost::system::error_code& ec)
		{
			return boost::asio::read(socket_, boost::asio::buffer(input), ec);
		}

		std::size_t read_impl(
			std::vector<char>& b,
			boost::system::error_code& ec)
		{
			return boost::asio::read(socket_, boost::asio::buffer(b), ec);
		}
	};

	template<typename InternetProtocol>
	SSLConnection_base<InternetProtocol>::~SSLConnection_base()
	{
	}

}