/**
 *	An asynchronous server class for handling multiple clients, without threads (Proactor pattern).
 * 
 *	Every time a client connects, a new session of type SessionType is started.
 */

#ifndef FRAMEWORK_SERVER_ASYNCSERVER_HPP
#define FRAMEWORK_SERVER_ASYNCSERVER_HPP

#include <ClientServerFramework/Server/Session.hpp>
#include <ClientServerFramework/Shared/SSL/SSL.hpp>

#include <boost/asio/io_service.hpp>

#include <memory>
#include <utility>

namespace Server {

	template<typename SessionType, typename Strategy>
	class AsyncServer
	{
	public:
		typedef Strategy strategy_type;

		/// ctor 
		/// IPv4 hard-coded for now
		AsyncServer(
			boost::asio::io_service& io_service, short port, 
			io::ssl_options const& SSL_options,
			strategy_type strategy) :
			strategy_(strategy),
			acceptor_(io_service, internet_protocol::endpoint(internet_protocol::v4(), port))
		{
			do_accept(port, SSL_options);
		}
	private:
		void do_accept(short port, io::ssl_options const& SSL_options)
		{
			// Create a new session. 
			SessionType::pointer_type new_session = 
				SessionType::create(acceptor_.get_io_service(), port, SSL_options, strategy_);


			// Set the connection to accept incoming client connections.
			// uses a lambda expression for the accept handler
			new_session->connection()->async_accept(acceptor_, 
				[this, new_session, port, SSL_options](boost::system::error_code const& ec)
			{
				if (!ec)
				{
					// Start the new session now that the client has successfully connected.
					new_session->start();
				}

				do_accept(port, SSL_options);
			});
		}

		strategy_type strategy_;

		typedef typename SessionType::internet_protocol internet_protocol;
		typedef typename SessionType::connection_type::acceptor_type acceptor_type;
		acceptor_type acceptor_;
	};

}	// namespace Server

#endif	// !FRAMEWORK_SERVER_ASYNCSERVER_HPP