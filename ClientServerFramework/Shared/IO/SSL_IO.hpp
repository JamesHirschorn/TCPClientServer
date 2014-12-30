/**
*	Concrete strategy for SSL encrypted communications.
*/

#ifndef FRAMEWORK_SHARED_IO_SSL_IO_HPP
#define FRAMEWORK_SHARED_IO_SSL_IO_HPP

#include <ClientServerFramework/Shared/IO/IO_base.hpp>
#include <ClientServerFramework/Shared/IO/SSL.hpp>

#include <boost/asio/ssl/context.hpp>
#include <boost/asio/ssl/stream.hpp>

#include <iostream>
#include <utility>

namespace io {

	template<typename InternetProtocol>
	class SSL_IO : public
		IO_base<InternetProtocol> 
	{
		typedef boost::asio::ssl::stream<typename internet_protocol::socket> socket_type;
		typedef typename socket_type::lowest_layer_type lowest_layer_type;
		typedef boost::asio::ssl::context context_type;
	public:
		/// ctor
		SSL_IO(
			boost::asio::io_service& io_service, 
			io::ssl_options const& SSL_options,
			side s) :
			context_(std::move(SSL_options.get_ssl_context())),
			socket_(io_service, context_),
			options_(SSL_options)
		{
			setup_socket(s);
		}

		/// Connect to the underlying socket (blocking).
		endpoint_iterator connect(
			endpoint_iterator begin,
			side s,
			boost::system::error_code& ec)
		{
			auto endpoint = boost::asio::connect(socket_.lowest_layer(), begin, ec);
			if (!ec)
			{
				// Perform SSL handshake immediately after openning the connection.
				handshake(s, ec);
				if (ec)
				{
					std::cerr << "SSL Handshake failed with error message: " << ec.message() << std::endl;

					throw ec;
				}
			}

			return endpoint;
		}

		/// Server initialization starts with the handshake.
		void async_initialize(initialize_handler const& handler)
		{
			async_handshake(SERVER, handler);
		}

		/// blocking handshake
		boost::system::error_code handshake(
			side s,
			boost::system::error_code & ec)
		{
			return socket_.handshake(get_handshake_type(s), ec);
		}
		/// asynchronous handshake
		void async_handshake(
			side s,
			async_handshake_handler const& handler)
		{
			socket_.async_handshake(get_handshake_type(s), handler);
		}

		/// asychronous acceptor
		void async_accept(
			acceptor_type& acceptor, 
			accept_handler const& handler)
		{
			acceptor.async_accept(socket_.lowest_layer(), handler);
		}

		/// setup client socket
		void setup_socket(side s)
		{
			if (s == CLIENT)
			{
				socket_.set_verify_mode(options_.verify_mode);
				socket_.set_verify_callback(
					[this](bool preverified, boost::asio::ssl::verify_context& ctx)
				{
					return ssl_verify_callback(preverified, ctx);
				});
			}
		}

		/* hooks */

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
		/// SSL context.
		context_type context_;
		/// The underlying socket.
		socket_type socket_;
		
		ssl_options options_;

		/// verify callback
		/// can be used, e.g. for verifying a certificate
		bool ssl_verify_callback(
			bool preverified, // True if the certificate passed pre-verification.
			boost::asio::ssl::verify_context& ctx // The peer certificate and other context.
			)
		{
			// In this example we will simply print the certificate's subject name.
			char subject_name[256];
			X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
			X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);
			std::cout << "Verifying certificate authority: " << subject_name << std::endl;
			return preverified;
		}

		/// convert side to handshake_type
		boost::asio::ssl::stream_base::handshake_type 
			get_handshake_type(side s)
		{
			switch (s)
			{
			case CLIENT:
				return boost::asio::ssl::stream_base::client;
			case SERVER:
				return boost::asio::ssl::stream_base::server;
			default:
				throw std::runtime_error("Undefined side.");
			}
		}
	};

}	// namespace IO

#endif // !FRAMEWORK_SHARED_IO_SSL_IO_HPP
