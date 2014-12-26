#include <ClientServerFramework/Shared/Serialization/SSLConnection.hpp>

namespace io {

	template<typename InternetProtocol>
	class SSLClientConnection :
		public SSLConnection<InternetProtocol>	
	{
	public:
		SSLClientConnection(
			boost::asio::io_service& io_service,
			ssl_options const& options) :
			SSLConnection<InternetProtocol>(io_service, options)
		{
			setup_socket();
		}

		// blocking handshake from Client
		boost::system::error_code handshake(boost::system::error_code & ec)
		{
			return socket().handshake(boost::asio::ssl::stream_base::client, ec);
		}

		/// dtor
		~SSLClientConnection() {}
	private:
		void setup_socket()
		{
			socket().set_verify_mode(options().verify_mode);
			socket().set_verify_callback(
				[this](bool preverified, boost::asio::ssl::verify_context& ctx)
			{
				return ssl_verify_callback(preverified, ctx);
			});
		}

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
			std::cout << "Verifying " << subject_name << std::endl;
			return preverified;
		}
	};

}	// namespace io