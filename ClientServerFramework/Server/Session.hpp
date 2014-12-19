#ifndef CLIENT_SERVER_SESSION_HPP
#define CLIENT_SERVER_SESSION_HPP

#include <memory>

#include <boost/asio/buffer.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>

#include <ClientServerFramework/Shared/Serialization/Connection.hpp>
#include <ClientServerFramework/Server/Response.hpp>

namespace ClientServer {
	
	/// Note: enabled_shared_from_this is used to ensure that the session remains alive
	///		  as long as some operation refers to it.
	template<typename InternetProtocol, typename Strategy, typename ClientData, typename ServerData>
	class Session :
		public std::enable_shared_from_this<Session<InternetProtocol, Strategy, ClientData, ServerData>>
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
			std::cerr << "Client connection with session id " << session_id_ << "." << std::endl;
			// Start receiving data from the client.
			do_receive();
		}

		/// socket inspector
		socket_type& socket()
		{
			return connection_.socket();
		}
	protected:
		// ctor
		Session(boost::asio::io_service& io_service) : 
			connection_(io_service)
		{
			session_id_ = count++;
			std::cerr << "Creating session " << session_id_ << std::endl;
		}
	public:
		// dtor
		~Session()
		{
			std::cerr << "Session " << session_id_ << " closing." << std::endl;
		}
	private:
		/// session counter
		static size_t count; 

		void do_receive()
		{
			using namespace boost::asio;

			//auto self(shared_from_this());

			//std::array<char, max_read_size> a;

			//boost::system::error_code ec;
			//size_t len = socket_.read_some(buffer(a), ec);

			//ClientData cdata;

			receive_helper helper(shared_from_this());

			// Capturing self with the lambda expression ensures that the Session remains alive
			// until the handler is called and completed (and possibly longer).
			connection_.async_read(cdata_, 
				[helper](boost::system::error_code const& ec, std::size_t length)
			{
				helper.handler(ec, length);
			});
				/*boost::bind(
					&Session::handle_receive,
					shared_from_this(),
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));*/
	/*			[this,&self,&cdata](boost::system::error_code const& ec, std::size_t length)
			{
				volatile pointer_type keep_alive(self);
				std::cerr << "Session " << session_id_ << " attempted to receive from client." << std::endl;
				if (!ec)
				{
					// perform the strategy on the received data
					response_.data = strategy_(cdata);
					response_.transmission_status = OK;

					// reply to the client with response_
					do_reply();
				}
			});*/
		}

		struct receive_helper
		{

			receive_helper(receive_helper const& other)
				: p_(other.p_)
			{
				std::cerr << "Shared pointer use count: " << p_.use_count() << ", session id: " << p_->session_id_ << std::endl;
			}

			explicit receive_helper(pointer_type const& p)
				: p_(p)
			{}

			void handler(boost::system::error_code const& ec, std::size_t length) const
			{
				p_->handle_receive(ec, length);
				/*if (!ec)
				{
					// perform the strategy on the received data
					Session::response_.data = strategy_(cdata_);
					response_.transmission_status = OK;

					// reply to the client with response_
					do_reply();
				}*/
			}

		private:
			pointer_type p_;
		};

		void handle_receive(boost::system::error_code const& ec, std::size_t length)
		{
			if (!ec)
			{
				// perform the strategy on the received data
				response_.data = strategy_(cdata_);
				response_.transmission_status = OK;

				// reply to the client with response_
				do_reply();
			}
		}

		void do_reply()
		{
			using namespace boost::asio;

			auto self(shared_from_this());

			connection_.async_write(response_,
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
		Strategy strategy_;			// The default constructed functor is used.
		ClientData cdata_;
		Response<ServerData> response_;
		std::size_t session_id_;
	};

	template<typename InternetProtocol, typename Strategy, typename ClientData, typename ServerData>
	std::size_t Session<InternetProtocol, Strategy, ClientData, ServerData>::count = 0;
}

#endif	// !CLIENT_SERVER_SESSION_HPP