/** 
 *	This is a generic Client Server session with 
 *	1. Asynchronous communication,
 *	2. Serialized data transfer.
 * 
 *	The entire communication session between a client and the server, between
 *	when the connection is made and when the connection is dropped, is 
 *  encapsulated in this object.
 */

#ifndef FRAMEWORK_SERVER_SESSION_HPP
#define FRAMEWORK_SERVER_SESSION_HPP

#include <memory>

#include <boost/asio/buffer.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>

#include <ClientServerFramework/Shared/Serialization/Connection.hpp>
#include <ClientServerFramework/Server/Response.hpp>

namespace Server {

	/// The Strategy Pattern is used here, as a functor given by a template parameter.
	/// It determines what action the server performs on the data sent from the client.
	/// Ideally, the strategy should be asynchronous, or only block for very short 
	/// durations, because otherwise is will block the single thread where all the 
	/// asynchronous communication occurs, and will thus block clients from communicating
	/// with the server.
	///
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
		typedef Strategy strategy_type;

		/// factory method (only way to creat new Session`s).
		static pointer_type create(boost::asio::io_service& io_service, strategy_type const& strategy)
		{
			return pointer_type(new Session(io_service, strategy));
		}

		void start()
		{
			// Session has does some very basic logging.
			std::clog << "Client connection with session id " << session_id_ << "." << std::endl;
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
		Session(boost::asio::io_service& io_service, strategy_type const& strategy) : 
			connection_(io_service),
			strategy_(strategy)
		{
			session_id_ = count++;
			std::clog << "Creating session " << session_id_ << std::endl;
		}
	public:
		// dtor
		~Session()
		{
			std::clog << "Session " << session_id_ << " closing." << std::endl;
		}
	private:
		/// session counter
		static size_t count; 

		void do_receive()
		{
			using namespace boost::asio;

			auto self(shared_from_this());

			// Capturing self with the lambda expression ensures that the Session remains alive
			// until the handler has completed executing (and possibly longer).
			connection_.async_read(cdata_, 
				[this,self](boost::system::error_code const& ec, std::size_t length)
			{
				std::clog << "Session " << session_id_ << " attempting to receive from client." << std::endl;
				if (!ec)
				{
					// perform the strategy on the received data
					response_.data = strategy_(cdata_);
					response_.transmission_status = OK;

					// reply to the client with response_
					do_reply();
				}
			});
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

		io::Connection<InternetProtocol> connection_;
		Strategy strategy_;			
		ClientData cdata_;
		Response<ServerData> response_;
		std::size_t session_id_;
	};

	template<typename InternetProtocol, typename Strategy, typename ClientData, typename ServerData>
	std::size_t Session<InternetProtocol, Strategy, ClientData, ServerData>::count = 0;
}	// namespace Server

#endif	// !FRAMEWORK_SERVER_SESSION_HPP