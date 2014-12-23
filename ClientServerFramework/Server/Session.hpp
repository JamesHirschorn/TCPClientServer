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

#include <ClientServerFramework/Server/Response.hpp>
#include <ClientServerFramework/Shared/Serialization/accepting_adaptor.hpp>
#include <ClientServerFramework/Shared/Serialization/Connection.hpp>

#include <boost/asio/buffer.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>

#include <memory>

namespace Server {

	/// The Strategy Pattern is used here, as a functor given by a template parameter.
	/// It determines what action the server performs on the data sent from the client.
	/// Ideally, the strategy should be asynchronous, or only block for very short 
	/// durations, because otherwise is will block the single thread where all the 
	/// asynchronous communication occurs, and will thus block clients from communicating
	/// with the server.
	///
	/// ClientData must have a std::string client_id field.
	template<typename InternetProtocol, typename Strategy, typename ClientData, typename ServerData>
	class Session :
		// used to ensure that the session remains alive as long as a callback is pending
		public std::enable_shared_from_this<Session<InternetProtocol, Strategy, ClientData, ServerData>>
	{
	public:
		typedef std::shared_ptr<Session> pointer_type;
		typedef Strategy strategy_type;
		typedef io::Connection<InternetProtocol> connection_type;

		/// factory method (only way to create new Session`s).
		static pointer_type create(boost::asio::io_service& io_service, short port,
			strategy_type const& strategy)
		{
			return pointer_type(new Session(io_service, port, strategy));
		}

		/// Starts up the session.
		void start()
		{
			// Session has doesome very basic logging.
			std::clog << "Client connection with session id " << session_id_ << "." << std::endl;
			// Start receiving data from the client.
			do_receive();
		}

		/// connection inspector
		connection_type& connection()
		{
			return connection_;
		}
		/// dtor
		~Session()
		{
			std::clog << "Session " << session_id_ << " closing." << std::endl;
		}
	protected:
		/// ctor
		Session(boost::asio::io_service& io_service, short port, strategy_type const& strategy) :
			connection_(io_service), 
			connected_(false),
			strategy_(strategy)
		{
			session_id_ = count++;
			std::clog << "Creating session " << session_id_ << std::endl;
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
				if (!ec)
				{
					if (!connected_)
					{
						std::clog << "Session " << session_id_ << ": Client ID is " 
							<< cdata_.client_id << '.' << std::endl;
						connected_ = true;
					}
					std::clog << "Session " << session_id_ << ": " << length << " bytes received." << std::endl;


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
					std::clog << "Session " << session_id_ << ": " << length << " bytes sent." << std::endl;

					// Go back into a receiving state.
					do_receive();
				}
			});
		}

		connection_type connection_;
		/// whether a connection with the client has been established yet
		bool connected_;
		Strategy strategy_;
		ClientData cdata_;
		Response<ServerData> response_;
		std::size_t session_id_;
	};

	template<typename InternetProtocol, typename Strategy, typename ClientData, typename ServerData>
	std::size_t Session<InternetProtocol, Strategy, ClientData, ServerData>::count = 0;
}	// namespace Server

#endif	// !FRAMEWORK_SERVER_SESSION_HPP