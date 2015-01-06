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

#include <ClientServerFramework/Server/Response.hpp>
#include <ClientServerFramework/Shared/Connection/ClientConnection.hpp>

#include <boost/asio/io_service.hpp>

#include <memory>
#include <stdexcept>
#include <string>

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

		/// Non-copy constructible.
		Connector(Connector const& other) = delete;

		/// opens a connection to the server
		bool open(boost::system::error_code& ec)
		{
			// Resolve the query to a list of end-points.
			// The end-points may contain both IPv4 and IPv6 end-points.
			endpoint_iterator endpoints = resolver_.resolve(query_);

			// Attempts to connect to each of the end-points until successful, 
			// or until all end-points have been tried.
			auto point = connection_->connect(endpoints, ec);

			// If successful, point will reference a valid end-point.
			return point != endpoint_iterator();
		}

		/// opens a connection to the server asynchronously
		/// (Needs re-implementation.)
		/*template<typename Handler>
		void async_open(Handler const& handler)
		{
		}*/

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
			sent_length = connection_->write(data, ec);

			response_type result;
			if (!ec)
			{
				received_length = connection_->read(result, ec);
			}

			return result;
		}
	protected:
		Connector(
			boost::asio::io_service& io_service, 
			io::ssl_options const& SSL_options, bool compression,
			std::string const& host, std::string const& service) :
			connection_(new connection_type(io_service, SSL_options, compression)), 
			resolver_(io_service),
			query_(host, service)
		{}
	private:
		typedef io::ClientConnection<InternetProtocol> connection_type;
		typedef typename connection_type::resolver_type resolver_type;
		typedef typename connection_type::query_type query_type;
		typedef typename connection_type::endpoint_iterator endpoint_iterator;

		/// The (serialized) connection to the server.
		std::shared_ptr<connection_type> connection_;
		resolver_type resolver_;
		query_type query_;
	};
}	// namespace Client

#endif	// !FRAMEWORK_CLIENT_CONNECTOR_HPP