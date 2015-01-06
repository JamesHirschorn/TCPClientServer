/** This is the strategy for the multiplication calculator client. */

#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>

/// ClientData must be client_data (see client_data.h), or a fully compatible type.
template<typename ClientData, typename Response>
struct ClientActions
{
	/// action to be performed on requests
	struct request_type
	{
		/// this meets the official standards for a function object
		typedef void result_type;

		/// output the data
		result_type operator()(ClientData const& d) const
		{
			using namespace std;

			cout << "Request Id: " << d.id 
				<< ". Request: " << std::setprecision(15)
				<< d.operand1 << ' ' << d.operation << ' ' << d.operand2 << '.' 
				<< endl << endl;

			// keep track of last padding
			padding = d.padding;
		}
	} request;

	struct response_type
	{
		typedef void result_type;

		result_type operator()(Response const& r) const
		{
			using namespace std;
			
			cout << "Request Id: " << r.data.id << '.' << endl
				<< "Result: " << std::setprecision(15) 
				<< r.data.result << ", computed at " << r.data.timestamp << '.'
				<< endl << endl;

			// Verify that the padding remains unchanged.
			assert(r.data.padding == padding); 
		}
	} response;
private:
	static std::array<char, client_data::padding_size> padding;
};

template<typename ClientData, typename Response>
std::array<char, client_data::padding_size> ClientActions<ClientData, Response>::padding;