/**
 *	An asynchronous server class for handling multiple clients, without threads (Proactor pattern).
 * 
 *	Every time a client connects, a new session of type SessionType is started.
 */

#include <memory>
#include <utility>

#include <boost/asio/io_service.hpp>

#include <ClientServerFramework/Server/Session.hpp>

namespace ClientServer {

	template<typename InternetProtocol, typename SessionType, typename Strategy>
	class AsyncServer
	{
		typedef typename InternetProtocol::endpoint endpoint_type;
	public:
		typedef Strategy strategy_type;

		/// ctor 
		/// IPv4 is hard-coded for now. 
		AsyncServer(
			boost::asio::io_service& io_service, short port, 
			strategy_type strategy) :
			acceptor_(io_service, endpoint_type(InternetProtocol::v4(), port)),
			socket_(io_service),
			strategy_(strategy)
		{
			do_accept();
		}
	private:
		void do_accept()
		{
			// Create a new session in anticipation of a client connection request.
			SessionType::pointer_type new_session = 
				SessionType::create(acceptor_.get_io_service(), strategy_);

			// uses a lambda expression for the accept handler
			acceptor_.async_accept(new_session->socket(),
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

		typedef typename InternetProtocol::acceptor acceptor_type;
		acceptor_type acceptor_;
		typedef typename InternetProtocol::socket socket_type;
		socket_type socket_;
		strategy_type strategy_;
	};

}