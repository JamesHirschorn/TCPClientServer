#include <exception>

#include <ClientServerStringReversal/TCPClient/DataScraper.h>

DataScraper::DataScraper(std::istream& is)
	: Scraper(is)
{
}

/// Define the abstract data getter.
data ClientServer::Scraper<data>::get_datum()
{
	using namespace std;

	data d;

	try
	{
		d.request << is_;
		d.id << is_;
		d.integer << is_;
		d.real << is_;
	}
	catch (std::exception& e)
	{
		/// Note: Should be checking what exception e is (in a rush).
		throw bad_data_exception();
	}

	return d;
}