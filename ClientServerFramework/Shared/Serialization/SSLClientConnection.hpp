#include <ClientServerFramework/Shared/Serialization/SSLConnection_base.hpp>

#include <boost/asio/ssl/stream_base.hpp>

namespace io {

	template<typename InternetProtocol>
	class SSLClientConnection :
		public SSLConnection_base < InternetProtocol >
	{
	public:
		SSLClientConnection(
			boost::asio::io_service& io_service,
			ssl_options const& options) :
			SSLConnection_base(io_service, options),
			options_(options)
		{}

		// blocking handshake from Client
		boost::system::error_code handshake(boost::system::error_code & ec)
		{
			return socket().handshake(boost::asio::ssl::stream_base::client, ec);
		}

		/// dtor
		~SSLClientConnection() {}
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
			context().load_verify_file(ca_filename);
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