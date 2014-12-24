#include <ClientServerFramework/Shared/Serialization/SSLConnection.hpp>

#include <boost/asio/ssl/stream_base.hpp>

namespace io {

	template<typename InternetProtocol>
	class SSLServerConnection :
		public ServerConnection_base<InternetProtocol>
	{
	public:
		SSLServerConnection(
			boost::asio::io_service& io_service,
			ssl_options const& options) :
			SSLConnection_base<InternetProtocol>(io_service, options),
			options_(options)
		{
			setup_socket();
			setup_context();
		}

		/// asynchronous handshake from Server
		void async_handshake(aync_handshake_handler const& handler)
		{
			socket().async_handshake(boost::asio::ssl::stream_base::server, handler); 
		}

		/// asnychronous accept
		void async_accept(acceptor_type& acceptor, acceptor_handle_type const& handler)
		{
			acceptor.async_accept(lowest_layer_socket(), handler);
		}

		/// dtor
		~SSLServerConnection() {}
	private:
		void set_socket()
		{
			socket().set_verify_mode(options_.verify_mode);
			socket().set_verify_callback(
				[this](bool preverified, boost::asio::ssl::verify_context& ctx)
			{
				return ssl_verify_callback(preverified, ctx);
			});
		}

		void set_context()
		{
			context().set_options(options().context_options);
			context().set_password_callback(get_password);
			context().use_private_key_file(options().get_certificate_full_pathname());
			context().use_tmp_dh_file(options().get_DH_full_pathname());
		}

		std::string get_password() const
		{
			return options().password; 
		}
		/// can be used, e.g. for verifying a certificate
		virtual bool ssl_verify_callback(
			bool preverified, // True if the certificate passed pre-verification.
			boost::asio::ssl::verify_context& ctx // The peer certificate and other context.
			)
		{
			// In this example we will simply print the certificate's subject name.
			char subject_name[256];
			X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
			X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);
			std::cout << "Verifying " << subject_name << std::endl;
			return preverified;
		}

		ssl_options options_;
	};

}	// namespace io