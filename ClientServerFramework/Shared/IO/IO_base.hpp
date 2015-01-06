/**
 *	Interface for the Stategy pattern for socket IO.
 */

#ifndef FRAMEWORK_SHARED_IO_IO_BASE_HPP
#define FRAMEWORK_SHARED_IO_IO_BASE_HPP

#include <ClientServerFramework/Shared/IO/SSL.hpp>

#include <boost/asio/buffer.hpp>

#include <array>
#include <functional>
#include <vector>

namespace io {

	enum side { CLIENT, SERVER };

	template<typename InternetProtocol, std::size_t headerLength = 8>
	class IO_base 
	{
	public:
		typedef InternetProtocol internet_protocol;
		typedef typename internet_protocol::resolver resolver_type;
		typedef typename resolver_type::query query_type;
		typedef typename resolver_type::iterator endpoint_iterator;
		typedef typename internet_protocol::acceptor acceptor_type;
		typedef std::function<void(boost::system::error_code const&, std::size_t)> async_handler;
		typedef std::function<void(boost::system::error_code const&)> initialize_handler;
		typedef std::function<void(boost::system::error_code const&)> async_handshake_handler; 
		typedef std::function<void(boost::system::error_code const&)> accept_handler;
		static std::size_t const header_length = headerLength;
		typedef std::array<char, header_length> inbound_header_type;

		/// Connect to the underlying socket (blocking).
		virtual endpoint_iterator connect(
			endpoint_iterator begin,
			side s,
			boost::system::error_code& ec) = 0;

		/// addition server initialization, if any 
		virtual void async_initialize(initialize_handler const& handler) = 0;

		/// blocking handshake
		virtual boost::system::error_code handshake(
			side s,
			boost::system::error_code& ec) = 0;

		/// asynchronous handshake
		virtual void async_handshake(
			side s,
			async_handshake_handler const& handler) = 0;

		/// asychronous acceptor
		virtual void async_accept(acceptor_type& acceptor, accept_handler const& handler) = 0;

		/// socket setup
		virtual void setup_socket(side s) = 0;

		/* Hooks, for the Template Method design pattern where
		the template methods are the I/O methods in Connection_base. */

		virtual void async_write_impl(
			std::vector<boost::asio::const_buffer> const&,
			async_handler const&) = 0;

		virtual void async_read_impl(
			inbound_header_type&,
			async_handler const&) = 0;

		virtual void async_read_impl(
			std::vector<char>&,
			async_handler const&) = 0;

		virtual std::size_t write_impl(
			std::vector<boost::asio::const_buffer> const& buffers,
			boost::system::error_code& ec) = 0;

		virtual std::size_t read_impl(
			inbound_header_type&,
			boost::system::error_code& ec) = 0;

		virtual std::size_t read_impl(
			std::vector<char>& b,
			boost::system::error_code& ec) = 0;
	};

}

#endif	// !FRAMEWORK_SHARED_IO_IO_BASE_HPP
