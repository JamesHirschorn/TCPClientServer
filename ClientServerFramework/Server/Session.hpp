#ifndef CLIENT_SERVER_SESSION_HPP
#define CLIENT_SERVER_SESSION_HPP

#include <memory>

#include <boost/asio/buffer.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>

#include <ClientServerFramework/Server/Response.h>

namespace ClientServer {
	
	/// Note: enabled_shared_from_this is used to ensure that the session remains alive
	///		  as long as some operation refers to it.
	template<typename InternetProtocol, typename Action>
	class Session :
		public std::enable_shared_from_this<Session<InternetProtocol, Action>>
	{
	protected:
		typedef typename InternetProtocol::socket socket_type;
	public:
		//Session(socket_type socket) : socket_(std::move(socket)) {}
		/// ctor for lvalue socket
		Session(socket_type const& socket) : 
			socket_(std::move(socket))	// sockets are non-copyable
		{}
		/// ctor for rvalue socket
		/*Session(socket_type&& socket) : 
			socket_(std::move(socket))
		{}*/

		void start()
		{
			// Start receiving data from the client.
			do_receive();
		}
	private:
		void do_receive()
		{
			using namespace boost::asio;

			auto self(shared_from_this());

			// for initial testing just read a string message, of maximum length 20
			static size_t const max_read_size = 20;
			std::array<char, max_read_size> a;
			
			// Caputuring self with the lambda expression ensures that the Session remains alive
			// until the handler is called and completed.
			async_read(socket_, buffer(a), 
				[this, self, a](boost::system::error_code const& ec, std::size_t length)
			{
				if (!ec)
				{
					std::copy(a.begin(), a.end(), response_.message.begin());

					// perform the strategy on the received data
					action_(response_.message);

					// reply to the client with response_
					do_reply();
				}
			});
		}

		void do_reply()
		{
			using namespace boost::asio;

			auto self(shared_from_this());

			async_write(socket_, buffer(response_.message),
				[this, self](boost::system::error_code const& ec, std::size_t length)
			{
				if (!ec)
				{
					// Go back into a receiving state.
					do_receive();
				}
			});
		}

		socket_type socket_;
		Action action_;
		Response response_;
	};

}

#endif	// !CLIENT_SERVER_SESSION_HPP