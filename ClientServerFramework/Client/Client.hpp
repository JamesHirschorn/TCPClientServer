/**
 *	This is the generic Client class. 
 *
 *	1. It launches a data Scraper in an independent background thread,
 *	   and the Scraper pushes the data onto the Queue in the order received.
 *
 *	2. In the calling thread (the one that calls the start method), a
 *	   connection with the server is established, and then the queue is 
 *	   listened to for requests.
 *
 *	3. Requests are acted on by the Client using the strategy pattern 
 *	   (possibly doing nothing).
 *
 *  4. The request are then (after possible modification) sent to server, and
 *     a server response is obtained. 
 *
 *	5. The server response is acted on according to the strategy pattern.
 *
 *	6. Any error handling of client communications is done here.
 */

#ifndef FRAMEWORK_CLIENT_CLIENT_HPP
#define FRAMEWORK_CLIENT_CLIENT_HPP

#include <iostream>
#include <istream>
#include <stdexcept>
#include <string>
#include <thread>

#include <boost/asio/error.hpp>
#include <boost/system/error_code.hpp>

#include <ClientServerFramework/Server/Response.hpp>
#include <ClientServerFramework/Shared/DesignPatterns/Singleton.hpp>

namespace Client {

	template<typename Connector, typename Scraper, typename Strategy>
	class Client
	{
	public:
		/// factory method (for convenience)
		static Client* create(
			boost::asio::io_service& io_service, 
			std::string const& client_id,
			std::string const& host, std::string const& service,
			std::istream& is, 
			Strategy const& strategy)
		{
			return new Client(io_service, client_id, host, service, is, strategy);
		}

		/// starts the client
		void start()
		{
			// Start the scraper.

			// The following code will not compile in VS2013:
			//std::thread t(&Scraper::start, std::ref(scraper_));
			// Thus we use Scraper as a FunctionObject instead 
			// (i.e. we don't call start directly).
			std::thread t(std::ref(scraper_));
			t.detach();

			// whether to try re-opening the server connection
			bool retry;

			do
			{
				// Open the connection to the server.
				if (!connector_.open())
				{
					std::string err_msg = "Unable to open connection.";
					std::cerr << err_msg << std::endl;
					throw std::runtime_error(err_msg);
				}

				// listen
				retry = listen();
			} while (retry);
		}
	private:
		/// ctor
		Client(
			boost::asio::io_service& io_service,
			std::string const& client_id,
			std::string const& host, std::string const& service,
			std::istream& is,
			Strategy const& strategy) :
			connector_(io_service, host, service),
			scraper_(is, client_id),
			strategy_(strategy)
		{}

		/// Listen to the Queue and process requests in order.
		/// Return value indicates whether to try to re-establish a connection 
		/// with the server.
		bool listen()
		{
			// a reference to the queue
			auto& queue = scraper_.queue();

			bool ok = true;

			do
			{
				if (!queue.empty())
				{
					using namespace boost::asio::error;

					// the first data element
					auto data = queue.front();

					// Perform the request part of the strategy.
					strategy_.request(data);

					// Send the data to the server.
					boost::system::error_code ec;
					std::size_t sent_length, received_length;	// # of bytes sent/received
					auto response = connector_.send(data, ec, sent_length, received_length);

					std::clog << sent_length << " bytes sent." << std::endl;
					std::clog << received_length << " bytes received." << std::endl;

					// Perform the response part of the strategy.
					strategy_.response(response);

					// error handling
					switch (ec.value())
					{
					case boost::system::errc::success:
						ok = true;

						// also check the status
						ok &= response.transmission_status == Server::OK;

						// If everything is OK then pop the data from the queue.
						if (ok)
							queue.pop();
						break;
					case connection_aborted:
					case connection_reset:
					case timed_out:
						ok = false;
						break;
					default:
						// we throw for unhandled exceptions
						throw boost::system::system_error(ec);
					}
				}
			} while (ok && scraper_);

			// With the current simple error-handling: 
			// We always retry connecting, so long as the scraper is in good status.
			return scraper_;
		}

		Connector connector_;
		Scraper scraper_;
		Strategy strategy_;
	};

}	// namespace Client

#endif	// !FRAMEWORK_CLIENT_CLIENT_HPP