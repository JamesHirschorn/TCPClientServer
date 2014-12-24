/**
 *	An asynchronous server class for handling multiple clients, without threads (Proactor pattern).
 * 
 *	Every time a client connects, a new session of type SessionType is started.
 */

#ifndef FRAMEWORK_SERVER_ASYNCSERVER_HPP
#define FRAMEWORK_SERVER_ASYNCSERVER_HPP

#include <memory>
#include <utility>

#include <boost/asio/io_service.hpp>

#include <ClientServerFramework/Server/Session.hpp>

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
			strategy_type strategy) :
			port_(port),
			strategy_(strategy),
			acceptor_(io_service, internet_protocol::endpoint(internet_protocol::v4(), port))
		{
			do_accept();
		}
	private:
		void do_accept()
		{
			// Create a new session in anticipation of a client connection request.
			SessionType::pointer_type new_session = 
				SessionType::create(acceptor_.get_io_service(), port_, strategy_);

			// uses a lambda expression for the accept handler
			acceptor_.async_accept(new_session->connection().socket(),
				[this,new_session](boost::system::error_code const& ec)
			{
				if (!ec)
				{
					// Start the new session now that the client has successfully connected.
					new_session->start();
				}

				do_accept();
			});
		}

		short port_;
		strategy_type strategy_;

		typedef typename SessionType::internet_protocol internet_protocol;
		typedef typename internet_protocol::acceptor acceptor_type;
		acceptor_type acceptor_;
	};

}	// namespace Server

#endif	// !FRAMEWORK_SERVER_ASYNCSERVER_HPP