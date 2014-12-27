#ifndef FRAMEWORK_SERIALIZATION_UNSECUREDIO_BASE_HPP
#define FRAMEWORK_SERIALIZATION_UNSECUREDIO_BASE_HPP

#include <ClientServerFramework/Shared/Serialization/IO_base.hpp>

namespace io {

	template<typename InternetProtocol>
	class UnsecuredIO : public 
		IO_base<InternetProtocol>
	{
		typedef typename internet_protocol::socket socket_type;
	public:
		/// ctor
		UnsecuredIO(boost::asio::io_service& io_service) :
			socket_(io_service)
		{}

		/// Connect to the underlying socket (blocking).
		endpoint_iterator connect(
			endpoint_iterator begin,
			boost::system::error_code& ec)
		{
			return boost::asio::connect(socket_, begin);
		}

		/// Server initialization does nothing for this strategy.
		void async_initialize(initialize_handler const& handler)
		{
			using namespace boost::system;

			error_code ec;

			handler(ec);
		}

		/// asychronous acceptor
		void async_accept(acceptor_type& acceptor, accept_handler const& handler)
		{
			acceptor.async_accept(socket_, handler);
		}

		/* implementation of abstract methods */

		void async_write_impl(
			std::vector<boost::asio::const_buffer> const& buffers,
			async_handler const& handler)
		{
			boost::asio::async_write(socket_, buffers, handler);
		}

		void async_read_impl(
			inbound_header_type& input,
			async_handler const& handler)
		{
			boost::asio::async_read(socket_, boost::asio::buffer(input), handler);
		}

		void async_read_impl(
			std::vector<char>& input,
			async_handler const& handler)
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
			inbound_header_type& input,
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
	private:
		/// The underlying socket.
		socket_type socket_;
	};

}	// namespace IO

#endif // !FRAMEWORK_SERIALIZATION_UNSECUREDIO_BASE_HPP
