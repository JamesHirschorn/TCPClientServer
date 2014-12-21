#include <exception>
#include <sstream>

#include "DataScraper.hpp"

DataScraper::DataScraper(std::istream& is, std::string const& client_id)
	: Scraper(is), client_id_(client_id)
{
}

/// Define the abstract data getter.
client_data DataScraper::get_datum() const
{
	static std::streamsize const max_input_length = 1000;
	char buffer[max_input_length];

	client_data d;
	d.client_id = client_id_;

	// Note: Exception handling is done in Scraper.hpp.
	input_stream().getline(buffer, max_input_length);
	// Read into a string stream rather than manipulate the input directly.
	std::istringstream datum_input(buffer);

	get_item(datum_input, d.id);
	get_item(datum_input, d.operand1);

	// Special handling for the operator_type is necessary.
	char op;
	get_item(datum_input, op);
	switch (op)
	{
	case '+':
		d.operation = client_data::ADD;
		break;
	case '-':
		d.operation = client_data::SUBTRACT;
		break;
	case '*':
		d.operation = client_data::MULTIPLY;
		break;
	case '/':
		d.operation = client_data::DIVIDE;
		break;
	default:
		// invalid operation type
		throw bad_data_exception();
	}

	get_item(datum_input, d.operand2);

	return d;
}
