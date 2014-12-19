#include <algorithm>
#include <string>

#include <boost/asio/buffer.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>

#include <ClientServerFramework/Server/Response.hpp>

namespace ClientServer
{
	template<typename InternetProtocol, typename ClientData, typename ServerData>
	class Connector
	{
		typedef typename Response<ServerData> response_type;
	public:
		Connector(
			boost::asio::io_service& io_service, std::string const& host, std::string const& service) :
			resolver_(io_service), 
			query_(host, service),
			socket_(io_service)
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
			auto point = boost::asio::connect(socket_, endpoints, ec);

			handle_error(ec);

			// If successful, point will reference a valid end-point.
			return point != endpoint_iterator_type();
		}

		response_type send(ClientData const& data)
		{
			using namespace boost::asio;

			boost::system::error_code ec;

			// for initial testing just send a string message
			write(socket_, buffer(data), ec);
			handle_error(ec);

			response_type result;

			// for initial testing just read a string message, of maximum length 20
			std::array<char, max_read_size> a;
			//read(socket_, buffer(a), ec);
			socket_.read_some(buffer(a), ec);
			handle_error(ec);

			// copy a to result
			result.data.resize(a.size());
			std::copy(a.begin(), a.end(), result.data.begin()); 
			result.transmission_status = OK;

			return result;
		}
	private:
		/// Hard-coded max read size.
		static size_t const max_read_size = 80;

		// Handles any errors, if any, encountered during a socket operation.
		void handle_error(boost::system::error_code const& ec)
		{
			if (ec)
			{
				throw boost::system::system_error(ec);
			}
		}

		typedef typename InternetProtocol::resolver resolver_type;
		resolver_type resolver_;
		typedef typename resolver_type::query query_type;
		query_type query_;
		typedef typename resolver_type::iterator endpoint_iterator_type;
		typedef typename InternetProtocol::socket socket_type;
		socket_type socket_;
	};
}