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
	memset(buffer, 0, max_input_length);

	client_data d;
	d.client_id = client_id_;

	// Note: Exception handling is done in Scraper.hpp.
	while (input_stream() && !input_stream().eof() && buffer[0] == 0)
	{
		input_stream().getline(buffer, max_input_length);
	}

	if (input_stream().bad())
		throw std::runtime_error("Input stream corrupted.");

	if (!buffer[0])
		throw out_of_data();

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
		throw bad_data_error();
	}

	get_item(datum_input, d.operand2);

	return d;
}
