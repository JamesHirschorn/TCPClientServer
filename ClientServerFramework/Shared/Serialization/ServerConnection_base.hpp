
#ifndef FRAMEWORK_SERIALIZATION_SERVER_CONNECTION_BASE_HPP
#define FRAMEWORK_SERIALIZATION_SERVER_CONNECTION_BASE_HPP

#include <ClientServerFramework/Shared/Serialization/Connection_base.hpp>

#include <boost/asio/io_service.hpp>

#include <functional>

namespace io {

	template<typename InternetProtocol>
	class ServerConnection_base : 
		public Connection_base<InternetProtocol>
	{
	public:
		typedef typename internet_protocol::acceptor acceptor_type;
		typedef std::function<void(boost::system::error_code const&)> acceptor_handler_type;

		/// asychronous acceptor
		virtual void async_accept(acceptor_type& acceptor, acceptor_handler_type const& handler) = 0;
	};

}

#endif // !FRAMEWORK_SERIALIZATION_SERVER_CONNECTION_BASE_HPP