/** This is the strategy for the multiplication calculator client. */

#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>

/// ClientData must be data (see data.h), or a fully compatible type.
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

			cout << "Request Id: " << d.id << message << '.' << endl
				<< "Request: " << d.request << ", real: " << std::setprecision(15)
				<< d.real << ", integer: " << d.integer << '.' << endl << endl;
		}

		request_type(std::string const& msg) : message(msg)
		{}

		std::string message;
	} request;

	struct response_type
	{
		typedef void result_type;

		result_type operator()(Response const& r) const
		{
			using namespace std;

			cout << r.data << endl << endl;
		}
	} response;

	ClientActions(std::string const& msg) 
		: request(msg)
	{}
};