/** This is the strategy for the multiplication calculator server. */

#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>

/// ServerData must be std::string or something highly compatible with it.
template<typename ClientData, typename ServerData>
struct ServerAction
{
	typedef ServerData result_type;	// this meets the official standards for a function object

	result_type operator()(ClientData const& d) const
	{
		double x = d.real * d.integer;

		result_type result;

		std::ostringstream oss;
		oss << "Result for request id \"" << d.id << "\" is: " << std::endl
			<< d.integer << " X " << std::setprecision(15) << d.real 
			<< " = " << x << '.' << std::endl << std::endl;
		result = oss.str();

		return result;
	}
};