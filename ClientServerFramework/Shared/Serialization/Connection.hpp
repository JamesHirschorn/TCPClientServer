/** 
 *	Concrete implementation of Connection_base<InternetProtocol>.
 */

#ifndef FRAMEWORK_SHARED_SERIALIZATION_CONNECTION_HPP
#define FRAMEWORK_SHARED_SERIALIZATION_CONNECTION_HPP

#include <ClientServerFramework/Shared/Serialization/Connection_base.hpp>

namespace io {

	template<typename InternetProtocol>
	class Connection :
		public Connection_base<InternetProtocol>
	{
		typedef InternetProtocol internet_protocol;
		typedef typename internet_protocol::socket socket_type;
	public:
		/// ctor
		Connection(boost::asio::io_service& io_service) :
			socket_(io_service)
		{}

		endpoint_iterator_type connect(
			endpoint_iterator_type begin,
			boost::system::error_code& ec)
		{
			return boost::asio::connect(socket_, begin, ec);
		}
	private:
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

}	// namespace io

#endif	// !FRAMEWORK_SHARED_SERIALIZATION_CONNECTION_HPP