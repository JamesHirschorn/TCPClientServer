/** 
 *	This is the generic Connector class for connecting clients to the Server.
 *	It uses synchronous communication.
 *	
 *	Its functions are:
 *	1. To establish a connection with the server.
 *	2. Provide a send function that sends one item of type ClientData to the
 *	   Server, and then receives a response of type Response from the Server.
 */

#ifndef FRAMEWORK_CLIENT_CONNECTOR_HPP
#define FRAMEWORK_CLIENT_CONNECTOR_HPP

#include <memory>
#include <string>

#include <boost/asio/buffer.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/ssl.hpp>

#include <ClientServerFramework/Shared/Serialization/Connection.hpp>
#include <ClientServerFramework/Server/Response.hpp>

namespace Client
{
	template<
		typename InternetProtocol, 
		// type of the data sent by the client - must be serializable
		typename ClientData,
		// see Response.hpp for the Response type parameter
		typename Response>
	class Connector
	{
		typedef Response response_type;
	public:
		typedef std::shared_ptr<Client<InternetProtocol, ClientData, Response>> pointer_type;

		/// Factory method for constructing a Client.
		static pointer_type create(
			boost::asio::io_service& io_service,
			std::string const& host, std::string const& service)
		{
			return pointer_type(new Connector(io_service, host, service));
		}

		/// Non-copy constructible.
		Connector(Connector const& other) = delete;

		/// opens a connection to the server
		bool open(boost::system::error_code& ec)
		{
			// Perfrom necessary socket setup, if any.
			set_socket();

			// Resolve the query to a list of end-points.
			// The end-points may contain both IPv4 and IPv6 end-points.
			endpoint_iterator_type endpoints = resolver_.resolve(query_);

			// Attempts to connect to each of the end-points until successful, 
			// or until all end-points have been tried.
			auto point = boost::asio::connect(connection_.socket(), endpoints, ec);

			// If successful, point will reference a valid end-point.
			return point != endpoint_iterator_type();
		}

		/// opens a connection to the server asynchronously
		template<typename Handler>
		void async_open(Handler const& handler)
		{
			// Resolve the query to a list of end-points.
			// The end-points may contain both IPv4 and IPv6 end-points.
			endpoint_iterator_type endpoints = resolver_.resolve(query_);

			// Attempts to connect to each of the end-points until successful, 
			// or until all end-points have been tried.
			boost::asio::async_connect(socket_, endpoints, 
				[](boost::system::error_code const& ec, endpoint_iterator_type iter)
			{
				handler(ec, iter);
			});
		}

		/// Sends one time of data to the server
		/// and then returns the server response.
		/// ec contains the error code of the send operation.
		/// sent_length is the numbrer of bytes sent.
		/// received_length is the number of bytes received in response.
		/// The return value is undefined in the case of an error.
		response_type send(ClientData const& data, 
			boost::system::error_code& ec, 
			std::size_t& sent_length, std::size_t& received_length)
		{
			// connection_ will automatically decode data 
			// that is read/writes from/to the underlying socket.
			sent_length = connection_.write(data, ec);

			response_type result;
			if (!ec)
			{
				received_length = connection_.read(result, ec);
			}

			return result;
		}
	protected:
		Connector(
			boost::asio::io_service& io_service, std::string const& host, std::string const& service) :
			connection_(io_service),
			resolver_(io_service),
			query_(host, service)
		{}
	private:
		/// used for verifying a certificate
		virtual bool ssl_verify_callback(
			bool preverified, // True if the certificate passed pre-verification.
			boost::asio::ssl::verify_context& ctx // The peer certificate and other context.
			)
		{
			// default implementation does nothing since we might not be using SSL
			return preverified;
		}
		/// socket set up
		virtual void set_socket()
		{
			// does nothing by default
		}

		/// The (serialized) connection to the server.
		io::Connection_base connection_;

		typedef typename InternetProtocol::resolver resolver_type;
		resolver_type resolver_;
		typedef typename resolver_type::query query_type;
		query_type query_;
		typedef typename resolver_type::iterator endpoint_iterator_type;
	};
}	// namespace Client

#endif	// !FRAMEWORK_CLIENT_CONNECTOR_HPP