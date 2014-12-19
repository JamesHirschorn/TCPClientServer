#ifndef CLIENT_SERVER_SESSION_HPP
#define CLIENT_SERVER_SESSION_HPP

#include <memory>

#include <boost/asio/buffer.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>

#include <ClientServerFramework/Server/Response.hpp>

namespace ClientServer {
	
	/// Note: enabled_shared_from_this is used to ensure that the session remains alive
	///		  as long as some operation refers to it.
	template<typename InternetProtocol, typename Action, typename ClientData, typename ServerData>
	class Session :
		public std::enable_shared_from_this<Session<InternetProtocol, Action, ClientData, ServerData>>
	{
	protected:
		typedef typename InternetProtocol::socket socket_type;
	public:
		typedef std::shared_ptr<Session> pointer_type;

		/// factory method
		static pointer_type create(boost::asio::io_service& io_service)
		{
			return pointer_type(new Session(io_service));
		}

		void start()
		{
			// Start receiving data from the client.
			do_receive();
		}

		/// socket inspector
		socket_type& socket()
		{
			return socket_;
		}
	protected:
		// ctor
		Session(boost::asio::io_service& io_service) : socket_(io_service)
		{
		}
	private:
		/// Hard-coded max read size.
		static size_t const max_read_size = 80;

		void do_receive()
		{
			using namespace boost::asio;

			auto self(shared_from_this());

			std::array<char, max_read_size> a;

			//boost::system::error_code ec;
			//size_t len = socket_.read_some(buffer(a), ec);

			// Capturing self with the lambda expression ensures that the Session remains alive
			// until the handler is called and completed.
			socket_.async_read_some(buffer(a), 
				[this, self, &a](boost::system::error_code const& ec, std::size_t length)
			{
				if (!ec)
				{
					response_.data.resize(a.size());
					std::copy(a.begin(), a.end(), response_.data.begin());

					// perform the strategy on the received data
					action_(response_.data);

					// reply to the client with response_
					do_reply();
				}
			});
		}

		void do_reply()
		{
			using namespace boost::asio;

			auto self(shared_from_this());

			async_write(socket_, buffer(response_.data),
				[this, self](boost::system::error_code const& ec, std::size_t length)
			{
				if (!ec)
				{
					// Go back into a receiving state.
					do_receive();
				}
			});
		}

		Connection connection_;
		Action action_;
		Response<Data> response_;
	};

}

#endif	// !CLIENT_SERVER_SESSION_HPP