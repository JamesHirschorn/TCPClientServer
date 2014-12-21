/** 
 *	This is the generic Connector class for connecting clients to the Server.
 *	
 *	Its functions are:
 *	1. To establish a connection with the server.
 *	2. Provide a send function that sends one item of type ClientData to the
 *	   Server, and then receives a response of type Response from the Server.
 */

#ifndef FRAMEWORK_CLIENT_CONNECTOR_HPP
#define FRAMEWORK_CLIENT_CONNECTOR_HPP

#include <string>

#include <boost/asio/buffer.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>

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
		Connector(
			boost::asio::io_service& io_service, std::string const& host, std::string const& service) :
			connection_(io_service),
			resolver_(io_service),
			query_(host, service)
		{}

		/// Non-copy constructible.
		//Connector(Connector const& other) = delete;

		/// opens a connection to the server
		bool open()
		{
			// Resolve the query to a list of end-points.
			// The end-points may contain both IPv4 and IPv6 end-points.
			endpoint_iterator_type endpoints = resolver_.resolve(query_);

			// Contains the error code, if any, set by a socket operation.
			boost::system::error_code ec;

			// Attempts to connect to each of the end-points until successful, 
			// or until all end-points have been tried.
			auto point = boost::asio::connect(connection_.socket(), endpoints, ec);

			handle_error(ec);

			// If successful, point will reference a valid end-point.
			return point != endpoint_iterator_type();
		}

		/// opens a connection to the server ansynchronously
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
		/// The return value is undefined in the case of an error.
		response_type send(ClientData const& data, boost::system::error_code& ec, std::size_t& length)
		{
			// connection_ will automatically decode data 
			// that is read/writes from/to the underlying socket.
			length = connection_.write(data, ec);

			response_type result;
			if (!ec)
			{
				connection_.read(result, ec);
			}

			return result;
		}
	private:
		/// Handles any errors, if any, encountered during a socket operation.
		/// For now, it just throws an exception.
		void handle_error(boost::system::error_code const& ec)
		{
			if (ec)
			{
				throw boost::system::system_error(ec);
			}
		}

		/// The (serialized) connection to the server.
		io::Connection<InternetProtocol> connection_;

		typedef typename InternetProtocol::resolver resolver_type;
		resolver_type resolver_;
		typedef typename resolver_type::query query_type;
		query_type query_;
		typedef typename resolver_type::iterator endpoint_iterator_type;
	};
}	// namespace Client

#endif	// !FRAMEWORK_CLIENT_CONNECTOR_HPP