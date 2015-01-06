/** This is the strategy for the multiplication calculator server. */

#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>

#include <boost/date_time/posix_time/posix_time.hpp>

/// ServerData must be server_data or something fully compatible with it.
template<typename ClientData, typename ServerData>
struct ServerAction
{
	typedef ServerData result_type;	// this meets the official standards for a function object

	result_type operator()(ClientData const& d) const
	{
		using namespace boost::posix_time;

		// alias
		double const& x(d.operand1);
		double const& y(d.operand2);
		
		double answer;
		
		switch (d.operation)
		{
		case ClientData::ADD:
			answer = x + y;
			break;
		case ClientData::SUBTRACT:
			answer = x - y;
			break;
		case ClientData::MULTIPLY:
			answer = x * y;
			break;
		case ClientData::DIVIDE:
			answer = x / y;
			break;
		default:
			throw std::logic_error("Invalid operation");
		}

		ptime timestamp(microsec_clock::local_time());

		result_type result;

		result.timestamp = timestamp;
		result.id = d.id;
		result.result = answer;
		result.padding = d.padding;

		return result;
	}
};