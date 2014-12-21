/** 
 *	Concrete class based on the instantiation Scraper<client_data> 
 *	of the Scraper template from the framework.
 *
 *	The abstract method get_datum must be defined. It returns an element of
 *	type client_data from the input stream, i.e. it reads the next data item
 *	from the stream.
 */

#include <string>

#include <ClientServerFramework/Client/Scraper.hpp>
#include <ClientServerCalculator/TCPClient/client_data.hpp>

class DataScraper :
	public Client::Scraper<client_data>
{
public:
	DataScraper(std::istream& is, std::string const& client_id); 
private:
	client_data get_datum() const;

	/// helper for get_datum
	template<typename T>
	void get_item(std::istream& is, T& item) const
	{
		if (is.good())
			is >> item;

		if (is.fail())
			throw bad_data_exception();	// bad data entry
		if (is.bad())
			throw std::runtime_error("I/O error on input stream.");
	}

	std::string client_id_;
};