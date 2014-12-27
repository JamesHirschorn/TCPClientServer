#include <ClientServerFramework/Shared/Serialization/ServerConnection_base.hpp>
#include <ClientServerFramework/Shared/Serialization/SSLConnection.hpp>

#include <boost/asio/ssl/stream_base.hpp>

namespace io {

	template<typename InternetProtocol>
	class SSLServerConnection :
		public ServerConnection_base<InternetProtocol>
	{
	public:
		/// Concrete component in the decorator pattern.
		typedef SSLConnection<internet_protocol> concrete_component_type;
		typedef std::shared_ptr<concrete_component_type> connection_pointer;
		typedef typename SSLConnection<internet_protocol>::async_handshake_handler_type async_handshake_handler_type;

		/// ctor
		SSLServerConnection(concrete_component_type* connection)
			: connection_(connection)
		{
		}

		/// Server initialization starts with the handshake.
		virtual void async_initialize(initialize_handler_type const& handler)
		{
			async_handshake(handler);
		}

		/// asynchronous handshake from Server
		void async_handshake(async_handshake_handler_type const& handler)
		{
			connection_->socket().async_handshake(boost::asio::ssl::stream_base::server, handler); 
		}

		/// asnychronous accept
		void async_accept(acceptor_type& acceptor, accept_handler_type const& handler)
		{
			acceptor.async_accept(connection_->lowest_layer_socket(), handler);
		}

		/// dtor
		~SSLServerConnection() {}
	private:
		/// as in the decorator pattern
		connection_pointer connection_;

		void set_socket()
		{
			socket().set_verify_mode(options_.verify_mode);
			socket().set_verify_callback(
				[this](bool preverified, boost::asio::ssl::verify_context& ctx)
			{
				return ssl_verify_callback(preverified, ctx);
			});
		}

		/* implementation of abstract methods */

		void async_write_impl(
			std::vector<boost::asio::const_buffer> const& buffers,
			async_handler const& handler)
		{
			connection_->async_write_impl(buffers, handler);
		}

		void async_read_impl(
			input_header_type& input,
			async_handler const& handler)
		{
			connection_->async_read_impl(input, handler);
		}

		void async_read_impl(
			std::vector<char>& input,
			async_handler const& handler)
		{
			connection_->async_read_impl(input, handler);
		}

		std::size_t write_impl(
			std::vector<boost::asio::const_buffer> const& buffers,
			boost::system::error_code& ec)
		{
			return connection_->write_impl(buffers, ec);
		}

		std::size_t read_impl(
			input_header_type& input,
			boost::system::error_code& ec)
		{
			return connection_->read_impl(input, ec);
		}

		std::size_t read_impl(
			std::vector<char>& b,
			boost::system::error_code& ec)
		{
			return connection_->read_impl(b, ec);
		}
	};

}	// namespace io