#include <string>

#include <boost/asio/connect.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/write.hpp>

namespace ClientServer
{
	template<typename Resolver, typename Socket, typename Data>
	class Connector
	{
	public:
		Connector(std::string const& host, std::string const& service) :
			resolver_(io_service_), 
			query_(host, service),
			socket_(io_service_)
		{}

		bool open()
		{
			try
			{
				// Resolve the query to a list of end-points.
				// The end-points may contain both IPv4 and IPv6 end-points.
				Resolver::iterator endpoints = resolver_.resolve(query_);

				// Attempts to connect to each of the end-points until successful, 
				// or until all end-points have been tried.
				auto point = boost::asio::connect(socket_, endpoints);

				return point != endpoints.end();
			}
			catch (boost::system::system_error& e)
			{
				std::cerr << e.what() << std::endl;
			}
		}

		Response send(Data const& data)
		{
			// for initial testing just send a string message
			char request[max_length];
			std::cin.getline(request, max_length);
			size_t request_length = std::strlen(request);
			boost::asio::write(s, boost::asio::buffer(request, request_length));
		}
	private:
		boost::asio::io_service io_service_;
		Resolver resolver_;
		typedef typename Resolver::query Query;
		Query query_;
		Socket socket_;
	};
}