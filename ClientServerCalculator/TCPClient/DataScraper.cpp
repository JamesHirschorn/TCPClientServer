#include <exception>

#include <ClientServerCalculator/TCPClient/DataScraper.h>

DataScraper::DataScraper(std::istream& is)
	: Scraper(is)
{
}

/// Define the abstract data getter.
data DataScraper::get_datum() const
{
	using namespace std;

	data d;

	try
	{
		input_stream() >> d.request;
		input_stream() >> d.id;
		input_stream() >> d.integer;
		input_stream() >> d.real;
	}
	catch (std::exception& e)
	{
		/// Note: Should be checking what exception e is (in a rush).
		std::cerr << e.what() << std::endl;
		throw bad_data_exception();
	}

	return d;
}