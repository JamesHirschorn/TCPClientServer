#include <exception>
#include <sstream>

#include <ClientServerCalculator/TCPClient/DataScraper.h>

DataScraper::DataScraper(std::istream& is)
	: Scraper(is)
{
}

/// Define the abstract data getter.
data DataScraper::get_datum() const
{
	static std::streamsize const max_input_length = 1000;
	char buffer[max_input_length];

	data d;

	// Note: Exception handling is done in Scraper.hpp.
	input_stream().getline(buffer, max_input_length);
		// Read into a string stream rather than manipulate the input directly.
		std::istringstream datum_input(buffer);

		get_item(datum_input, d.request);
		get_item(datum_input, d.id);
		get_item(datum_input, d.integer);
		get_item(datum_input, d.real);

	return d;
}

template<typename T>
void DataScraper::get_item(std::istream& is, T& item) const
{
	if (is.good())
		is >> item;

	if (is.fail())
		throw bad_data_exception();	// bad data entry
	if (is.bad())
		throw std::runtime_error("I/O error on input stream.");
}