
#ifndef FRAMEWORK_SERIALIZATION_CLIENT_CONNECTION_BASE_HPP
#define FRAMEWORK_SERIALIZATION_CLIENT_CONNECTION_BASE_HPP

#include <ClientServerFramework/Shared/Serialization/Connection_base.hpp>

#include <boost/asio/io_service.hpp>

#include <functional>

namespace io {

	template<typename InternetProtocol>
	class ClientConnection_base :
		public Connection_base<InternetProtocol>
	{
	public:
		typedef typename internet_protocol::resolver resolver_type;
		typedef typename resolver_type::iterator endpoint_iterator_type;
		/*	typedef typename internet_protocol::acceptor acceptor_type;
		typedef std::function<void(boost::system::error_code const&)> initialize_handler_type;
		typedef std::function<void(boost::system::error_code const&)> accept_handler_type; */

		/// Connect to the underlying socket (blocking).
		virtual endpoint_iterator_type connect(
			endpoint_iterator_type begin,
			boost::system::error_code& ec) = 0;
	};

}

#endif // !FRAMEWORK_SERIALIZATION_CLIENT_CONNECTION_BASE_HPP