/** Handle SSL options, including setting up the SSL context. */

#ifndef FRAMEWORK_SHARED_IO_SSL_HPP
#define FRAMEWORK_SHARED_IO_SSL_HPP

#include <boost/asio/ssl/context.hpp>

#include <functional>
#include <stdexcept>

namespace io {

	enum ssl_mode { OFF, SSLV23, SSLV3 };

	struct ssl_options
	{
		ssl_mode							mode;
		long								context_options;
		boost::asio::ssl::verify_mode		verify_mode;
		std::string							certificate_path;
		std::string							certificate_filename;
		std::string							private_key_filename;
		std::string							password;
		std::string							DH_filename;

		/// gets the context determined by the options
		boost::asio::ssl::context get_ssl_context() const
		{
			boost::asio::ssl::context context(get_method());
			context.set_options(context_options);
			if (!certificate_filename.empty())
				context.load_verify_file(get_full_pathname(certificate_filename));
			if (!private_key_filename.empty())
			{
				context.set_password_callback(
					[this](std::size_t max_size, 
					boost::asio::ssl::context::password_purpose purpose)
				{ 
					return get_password(max_size, purpose); 
				});
				context.use_certificate_chain_file(get_full_pathname(certificate_filename));
				context.use_private_key_file(
					get_full_pathname(private_key_filename),
					boost::asio::ssl::context::pem);
			}
			if (context_options & boost::asio::ssl::context::single_dh_use)
				context.use_tmp_dh_file(get_full_pathname(DH_filename));

			return context;
		}

		boost::asio::ssl::context::method get_method() const
		{
			switch (mode)
			{
			case OFF:
				throw std::runtime_error("SSL is disabled.");
			case SSLV23:
				return boost::asio::ssl::context::sslv23;
				break;
			case SSLV3:
				return boost::asio::ssl::context::sslv3;
				break;
			default:
				throw std::runtime_error("Invalid SSL mode.");
			}
		}
	private:
		std::string get_full_pathname(std::string const& filename) const
		{
			return certificate_path + '/' + filename;
		}
		std::string get_password(
			std::size_t max_size, // maximum password size
			boost::asio::ssl::context::password_purpose purpose) const
		{
			if (password.size() <= max_size)
				return password;

			throw std::runtime_error("Password too long.");
		}
	};

}	// namespace io

#endif	// !FRAMEWORK_SHARED_IO_SSL_HPP