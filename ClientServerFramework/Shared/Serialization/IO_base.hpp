/**
 *	Interface for the Stategy pattern for socket IO.
 */

#ifndef FRAMEWORK_SERIALIZATION_IO_BASE_HPP
#define FRAMEWORK_SERIALIZATION_IO_BASE_HPP

#include <ClientServerFramework/Shared/Serialization/UnsecuredIO.hpp>
#include <ClientServerFramework/Shared/SSL/SSL.hpp>

//#include <boost/asio/io_service.hpp>

#include <array>
#include <functional>
#include <vector>

namespace io {

	template<typename InternetProtocol>
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
		typedef std::function<void(boost::system::error_code const&)> accept_handler;
		static std::size_t const header_length = 8;
		typedef std::array<char, header_length> inbound_header_type;

		/// factory method for IO_base_type
		static IO_base* create(
			boost::asio::io_service& io_service,
			io::ssl_options const& SSL_options)
		{
			IO_base* io;

			switch (SSL_options.mode)
			{
			case OFF:
				io = new UnsecuredIO<internet_protocol>(io_service);
				break;
			case SSLV23:
			case SSLV3:
				//conn = new SSLClientConnection<InternetProtocol>(io_service, SSL_options);
				break;
			default:
				throw std::runtime_error("Invalid SSL mode.");
			}

			return io;
		}

		/// Connect to the underlying socket (blocking).
		virtual endpoint_iterator connect(
			endpoint_iterator begin,
			boost::system::error_code& ec) = 0;

		/// addition server initialization, if any 
		virtual void async_initialize(initialize_handler const& handler) = 0;

		/// asychronous acceptor
		virtual void async_accept(acceptor_type& acceptor, accept_handler const& handler) = 0;

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

#endif	// !FRAMEWORK_SERIALIZATION_IO_BASE_HPP