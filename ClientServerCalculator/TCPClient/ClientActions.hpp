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
		}
	} response;
};