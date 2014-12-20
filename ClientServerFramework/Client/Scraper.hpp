/**
 * Abstract base class for scraping data from the input.
 */

#include <exception>
#include <iostream>
#include <istream>

#include <ClientServerFramework/Shared/Container/SafeQueue.hpp>
#include <ClientServerFramework/Shared/DesignPatterns/Singleton.hpp>

namespace ClientServer
{
	/// Scrapes and confirms the input, and then pushes it 
	/// on the Singeton instance of the Queue.
	template<
		typename Data, 
		// use a thread-safe Queue class by default
		typename Queue = safe_container::queue<Data>>	
	class Scraper
	{
		typedef Queue queue_type;
	protected:
		/// ctor
		Scraper(std::istream& is) :
			is_(is), 
			queue_(patterns::Singleton<queue_type>::get_instance())
		{
			std::clog << "Data scraper started." << std::endl;
		}

		/// input stream inspector
		std::istream& input_stream() const
		{
			return is_;
		}

		/// thrown when attempt to read one item of data fails
		class bad_data_exception : public std::exception
		{};
	public:
		void start() const
		{
			while(is_)
			{
				Data d;

				try
				{
					Data d = get_datum();
					// push onto the queue
					write_datum(d);
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
		}

		/// Allows Scraper to also be used as a FunctionObject (see the C++11 standard).
		void operator()() const
		{
			start();
		}

		/// whether the scraper is in a good state
		operator bool() const
		{
			return is_.good();
		}

		/// dtor
		virtual ~Scraper() 
		{
			std::clog << "Scraper terminating." << std::endl;
		}

		/// Queue inspector
		queue_type& queue() const
		{
			return queue_;
		}
	private:
		/// Abstract member for reading one item of data from the input stream.
		virtual Data get_datum() const = 0;

		// We lock the queue before writing to it, since the queue will be 
		// accessed outside this thread. 
		void write_datum(Data const& d) const
		{
			queue_.push(d);
		}

		std::istream& is_;
		queue_type& queue_;
	};

}

