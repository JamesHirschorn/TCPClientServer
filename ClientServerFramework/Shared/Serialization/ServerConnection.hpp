#include <ClientServerFramework/Shared/Serialization/ServerConnection_base.hpp>

#include <boost/asio/io_service.hpp>

#include <memory>

namespace io {

	template<typename InternetProtocol>
	class ServerConnection : 
		public ServerConnection_base<InternetProtocol>
	{
	public:
		/// Concrete component in the decorator pattern.
		typedef Connection<internet_protocol> concrete_component_type;
		typedef std::shared_ptr<concrete_component_type> connection_pointer;

		ServerConnection(concrete_component_type* connection)
			: connection_(connection)
		{
		}

		/// Connect to the underlying socket (blocking).
		endpoint_iterator_type connect(
			endpoint_iterator_type begin,
			boost::system::error_code& ec)
		{
			return connection_->connect(begin, ec);
		}

		/// asychronous acceptor
		void async_accept(acceptor_type& acceptor, acceptor_handler_type const& handler)
		{
			acceptor.async_accept(connection_->socket(), handler);
		}
	private:
		/// as in the decorator pattern
		connection_pointer connection_;

		/* implementation of abstract methods */

		void async_write_impl(
			std::vector<boost::asio::const_buffer> const& buffers,
			async_handler_type const& handler)
		{
			connection_->async_write_impl(buffers, handler);
		}

		void async_read_impl(
			input_header_type& input,
			async_handler_type const& handler)
		{
			connection_->async_read_impl(input, handler);
		}

		void async_read_impl(
			std::vector<char>& input,
			async_handler_type const& handler)
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

}