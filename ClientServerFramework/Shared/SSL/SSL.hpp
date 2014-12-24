#ifndef FRAMEWORK_SHARED_SSH_HPP
#define FRAMEWORK_SHARED_SSH_HPP

#include <boost/asio/ssl/context.hpp>

namespace io {

	enum ssl_mode { OFF, SSLV23 };

	struct ssl_options
	{
		ssl_mode							mode;
		boost::asio::ssl::verify_mode		verify_mode;
		std::string							certificate_path;
		std::string							certificate_filename;

		boost::asio::ssl::context::method get_method() const
		{
			switch (mode)
			{
			case OFF:
				throw std::runtime_error("SSL is disabled.");

			case SSLV23:
				return boost::asio::ssl::context::sslv23;
				break;
			default:
				throw std::runtime_error("Invalid SSL mode.");
			}
		}
	};

}	// namespace io

#endif	// !FRAMEWORK_SHARED_SSH_HPP