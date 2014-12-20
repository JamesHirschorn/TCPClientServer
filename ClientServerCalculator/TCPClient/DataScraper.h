/** 
 *	Concrete class based on the Scraper<Data> class from the framework.
 *
 *	The abstract method get_datum must be defined. It returns an element of
 *	type Data from the input stream, i.e. it read the next data item from the
 *	stream.
 */

#include <ClientServerFramework/Client/Scraper.hpp>

#include <ClientServerCalculator/Shared/Data.h>

class DataScraper :
	public ClientServer::Scraper<data>
{
public:
	DataScraper(std::istream& is); 
private:
	data get_datum() const;
};