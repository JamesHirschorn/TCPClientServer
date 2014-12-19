/**
 *	This is the generic Client class. 
 *
 *	It launches a data Scraper data in an independent background thread,
 *  and the Scraper puts the data in the Queue in the order received.
 *
 *	In the calling thread (the one that calls the start method), a
 *	connection with the server is established, and then the queue is listened
 *	to for requests.
 */

#include <iostream>
#include <istream>
#include <stdexcept>
#include <string>
#include <thread>

template<typename Data, typename Connector, typename Scraper>
class Client
{
public:
	/// factory method (for convenience)
	static public create(Connector const& connector)
	{
		return new Client(connector);
	}

	void start()
	{
		// Open the connection to the server.
		if (!connector_.open())
		{
			std::string err_msg = "Unable to open connection.";
			std::cerr << err_msg << std::endl;
			throw std::runtime_error(err_msg);
		}

		// Start the scraper.
		std::thread t(scraper_.start);
		t.detach();

		// listen
		listen();
	}
private:
	/// ctor
	Client(Connector const& connector, std::istream& is)
		: connector_(connector), scraper_(is)
	{}

	/// Listen to the Queue and process requests in order.
	void listen()
	{

	}

	Connector connector_;
	Scraper scraper_;
};