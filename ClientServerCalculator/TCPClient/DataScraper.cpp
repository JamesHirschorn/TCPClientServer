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
	using namespace std;

	static std::streamsize const max_input_length = 1000;
	char buffer[max_input_length];

	data d;

	try
	{
		input_stream().getline(buffer, std::numeric_limits<std::streamsize>::max());
		istringstream datum_input(buffer);

		get_item(datum_input, d.request);
		get_item(datum_input, d.id);
		get_item(datum_input, d.integer);
		get_item(datum_input, d.real);
	}
	catch (bad_data_exception& e)
	{
		throw e;
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		throw e;
	}

	return d;
}

template<typename T>
void DataScraper::get_item(std::istream& is, T& item) const
{
	if (is.good())
		is >> item;

	if (is.fail())
		throw bad_data_exception();
	if (is.bad())
		throw std::runtime_error("I/O error on input stream.");
	//T t;
	//if (input_stream().good())
	//{
	//	std::cin >> item;
	//	//item = t;
	//}
		//input_stream() >> item;
			//std::cin >> item;
	//for(;;) {
	//	input_stream() >> item;

	//	if (input.eof() || input.bad()) {
	//		break;
	//	} else if (input.fail()) {
	//		input.clear(); // unset failbit
	//		input.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // skip bad input
	//	} else {
	//		std::cout << n << '\n';
	//	}
	//}
	//std::cin >> item;
	
	//if (!input_stream().good())
	//	// can be caused, e.g., by incomplete data item from input
	//	throw bad_data_exception();	
}