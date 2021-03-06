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
 *
 *	7. In its current form, it is only suitable for synchronous communication
 *	   with the server; however, it could be easily be modified to also allow 
 *	   for asynchronous Connector`s as well.
 */

#ifndef FRAMEWORK_CLIENT_CLIENT_HPP
#define FRAMEWORK_CLIENT_CLIENT_HPP

#include <ClientServerFramework/Server/Response.hpp>
#include <ClientServerFramework/Shared/DesignPatterns/Singleton.hpp>
#include <ClientServerFramework/Shared/IO/SSL.hpp>

#include <boost/asio/io_service.hpp>
#include <boost/system/error_code.hpp>

#include <iostream>
#include <istream>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>

namespace Client {

	template<typename Connector, typename Scraper, typename Strategy>
	class Client
	{
	public:
		typedef std::shared_ptr<Client> pointer_type;

		/// factory method 
		/// keep_alive indicates whether to keep the session alive 
		/// (from the client end) when the queue is empty and 
		/// the input stream is in the EOF state. This would be set to true
		/// for cin, but possibly false for a file stream.
		static pointer_type create(
			boost::asio::io_service& io_service, 
			io::ssl_options const& SSL_options,
			bool compression,
			std::string const& client_id,
			std::string const& host, std::string const& service,
			std::istream& is, bool keep_alive, 
			Strategy const& strategy)
		{
			return pointer_type(
				new Client(io_service, SSL_options, compression,
					client_id, host, service, is, keep_alive, strategy));
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
				boost::system::error_code ec;

				// Open the connection to the server.
				bool open = connector_.open(ec);
				default_error_handler(ec);
				if (!open)
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
			io::ssl_options const& SSL_options,
			bool compression,
			std::string const& client_id,
			std::string const& host, std::string const& service,
			std::istream& is, bool keep_alive,
			Strategy const& strategy) :
			connector_(io_service, SSL_options, compression, host, service),
			scraper_(is, client_id),
			keep_alive_(keep_alive),
			strategy_(strategy)
		{}

		/// Listen to the Queue and process requests in order.
		/// Return value indicates whether to try to re-establish a connection 
		/// with the server.
		bool listen()
		{
			// a reference to the queue
			auto& queue = scraper_.queue();

			bool connection_ok = true;
			bool input_ok = true;	// described below

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

					// error handling
					switch (ec.value())
					{
					case boost::system::errc::success:
						connection_ok = true;

						// also check the status
						connection_ok &= response.transmission_status == Server::OK;

						// If everything is OK then handle the response, 
						// and then pop the data from the queue.
						if (connection_ok)
						{
							std::clog << sent_length << " bytes sent." << std::endl;
							std::clog << received_length << " bytes received." << std::endl;

							// Perform the response part of the strategy.
							strategy_.response(response);

							queue.pop();
						}
						break;
					case connection_aborted:
					case connection_reset:
					case timed_out:
						connection_ok = false;
						break;
					default:
						default_error_handler(ec);
						break;
					}
				}

				// With the current simple error-handling: 
				// We always retry connecting, so long as 
				// 1. the scraper is in good status, OR
				// 2. there is still data in the queue, OR
				// 3. keep_alive_;
				// i.e. either keep_alive is set 
				// or else we are not out of data yet.
				input_ok = keep_alive_ || !queue.empty() || scraper_;
			} while (connection_ok && input_ok);	

			return input_ok;
		}

		/// For now, it just throws an exception.
		void default_error_handler(boost::system::error_code const& ec)
		{
			if (ec)
			{
				throw boost::system::system_error(ec);
			}
		}

		Connector connector_;
		Scraper scraper_;
		bool keep_alive_;
		Strategy strategy_;
	};

}	// namespace Client

#endif	// !FRAMEWORK_CLIENT_CLIENT_HPP