/**
 * Abstract base class for scraping data from the input.
 */

#include <exception>
#include <iostream>
#include <istream>
#include <mutex>
#include <queue>

#include <ClientServerFramework/Shared/DesignPatterns/Singleton.hpp>

namespace ClientServer
{

	template<typename Data, template<typename> class Queue = std::queue<Data>>
	class Scraper
	{
	protected:
		/// ctor
		Scraper(std::istream& is)
			: is_(is), queue_(patterns::Singleton<Queue>.get_instance())
		{}

		/// thrown when attempt to read one item of data fails
		class bad_data_exception : public std::exception
		{};
	public:
		void start()
		{
			while(is_)
			{
				Data d;

				try
				{
					d = get_datum(is_);
				}
				catch (bad_data_exception)
				{
					// Ignore this bad data item and continue scraping.
					continue;
				}
				catch (std::exception& e)
				{
					std::cerr << "Error in scraping input data: " << e.what() << std::endl;
					break;
				}
			}

			// thread-safe write to the queue
			write_datum(d);
		}

		/// dtor
		virtual ~Scraper() {}
	private:
		/// Abstract member for reading one item of data.
		virtual Data get_datum() = 0;

		// We lock the queue before writing to it, since the queue will be 
		// accessed outside this thread. 
		void write_datum(Data const& d)
		{
			std::lock_guard<std::mutex> lock(locker_);
			queue_.push(d);
		}

		std::istream& is_;
		Queue& queue_;
		std::mutex locker_;
	};

}

