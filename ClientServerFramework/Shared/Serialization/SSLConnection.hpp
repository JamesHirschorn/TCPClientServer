#include <ClientServerFramework/Shared/Serialization/Connection_base.hpp>

#include <boost/asio/io_service.hpp>
#include <boost/asio/ssl/context.hpp>
#include <boost/asio/ssl/stream.hpp>

#include <stdexcept>

namespace io {

	template<typename InternetProtocol>
	class SSLConnection :
		public Connection_base<InternetProtocol>
	{
		typedef InternetProtocol internet_protocol;
		typedef boost::asio::ssl::stream<typename internet_protocol::socket> socket_type;
		typedef typename socket_type::lowest_layer_type lowest_layer_type;
		typedef boost::asio::ssl::context context_type;
	public:
		typedef std::function<void(boost::system::error_code const&)> async_handshake_handler;

		SSLConnection(
			boost::asio::io_service& io_service,
			ssl_options const& options) :
			context_(options.get_method()),
			socket_(io_service, context_),
			options_(options)
		{
		}

		endpoint_iterator_type connect(
			endpoint_iterator_type begin,
			boost::system::error_code& ec)
		{
			endpoint_iterator_type endpoint =
				boost::asio::connect(lowest_layer_socket(), begin, ec);

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

		/// blocking handshake
		virtual boost::system::error_code handshake(boost::system::error_code& ec)
		{
			// need this class to be concrete, so not pure virtual
			throw std::logic_error("Not implemented.");
		}

		/// asynchronous handshake
		void async_handshake(async_handshake_handler const& handler)
		{
			// need this class to be concrete, so not pure virtual
			throw std::logic_error("Not implemented.");
		}

		/// dtor
		~SSLConnection() {}
	protected:
		/// socket inspectors
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
		/// options inspector
		ssl_options options() const
		{
			return options_;
		}
	private:
		context_type context_;

		/// the underlying socket
		socket_type socket_;

		ssl_options options_;

		/// set up the socket (does nothing by default)
		virtual void setup_socket() {}
		/// set up the context (does nothing by default)
		virtual void setup_context() {};

		/* implementation of abstract methods */

		void async_write_impl(
			std::vector<boost::asio::const_buffer> const& buffers,
			async_handler_type const& handler)
		{
			boost::asio::async_write(socket(), buffers, handler);
		}

		void async_read_impl(
			input_header_type& input,
			async_handler_type const& handler)
		{
			boost::asio::async_read(socket(), boost::asio::buffer(input), handler);
		}

		void async_read_impl(
			std::vector<char>& input,
			async_handler_type const& handler)
		{
			boost::asio::async_read(socket(), boost::asio::buffer(input), handler);
		}

		std::size_t write_impl(
			std::vector<boost::asio::const_buffer> const& buffers,
			boost::system::error_code& ec)
		{
			return boost::asio::write(socket(), buffers, boost::asio::transfer_all(), ec);
		}

		std::size_t read_impl(
			input_header_type& input,
			boost::system::error_code& ec)
		{
			return boost::asio::read(socket(), boost::asio::buffer(input), ec);
		}

		std::size_t read_impl(
			std::vector<char>& b,
			boost::system::error_code& ec)
		{
			return boost::asio::read(socket(), boost::asio::buffer(b), ec);
		}
	};

}	// namespace io