#include <memory>
#include <utility>

#include <boost/asio/io_service.hpp>

#include <ClientServerFramework/Server/Session.hpp>

namespace ClientServer {

	template<typename InternetProtocol>
	class AsyncServer
	{
	public:
		AsyncServer(boost::asio::io_service& io_service, short port) :
			acceptor_(io_service, InternetProtocol::v4(), port),
			socket_(io_service)
		{
			do_accept();
		}
	private:
		void do_accept()
		{
			// uses a lambda expression for the accept handler
			acceptor_.async_accept(socket_,
				[this](boost::system::error_code const& ec)
			{
				if (!ec)
				{
					// C++11 way of spawning a new Session from a socket.
					// socket_ will be independent of Session afterwards.
					// This is much more succinct and (I believe) efficient than
					// the "old-fashioned way" of created a new socket by hand 
					// for each new Session. 
					std::make_shared<Session>(std::move(socket_))->start();
				}

				do_accept();
			});
		}

		typedef typename InternetProtocol::acceptor acceptor_type;
		acceptor_type acceptor_;
		typedef typename InternetProtocol::socket socket_type;
		socket_type socket_;
	};

}