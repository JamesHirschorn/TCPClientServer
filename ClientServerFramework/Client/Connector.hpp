#include <algorithm>
#include <string>

#include <boost/asio/buffer.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>

#include <ClientServerFramework/Shared/Serialization/Connection.hpp>
#include <ClientServerFramework/Server/Response.hpp>

namespace ClientServer
{
	template<typename InternetProtocol, typename ClientData, typename Response>
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

		void async_open()
		{
			// Resolve the query to a list of end-points.
			// The end-points may contain both IPv4 and IPv6 end-points.
			endpoint_iterator_type endpoints = resolver_.resolve(query_);

			// Attempts to connect to each of the end-points until successful, 
			// or until all end-points have been tried.
			boost::asio::async_connect(socket_, endpoints, 
				[&endpoint](boost::system::error_code const& ec, endpoint_iterator_type iter)
			{
				handle_error(ec);
			});
		}

		response_type send(ClientData const& data)
		{
			using namespace boost::asio;

			boost::system::error_code ec;

			// connection_ will automatically decode data that is read from the underlying socket.
			connection_.write(data, ec);
			handle_error(ec);

			response_type result;
			connection_.read(result, ec);
			handle_error(ec);

			return result;
		}
	private:
		// Handles any errors, if any, encountered during a socket operation.
		void handle_error(boost::system::error_code const& ec)
		{
			if (ec)
			{
				throw boost::system::system_error(ec);
			}
		}

		/// The connection to the server.
		Connection<InternetProtocol> connection_;

		typedef typename InternetProtocol::resolver resolver_type;
		resolver_type resolver_;
		typedef typename resolver_type::query query_type;
		query_type query_;
		typedef typename resolver_type::iterator endpoint_iterator_type;
	};
}