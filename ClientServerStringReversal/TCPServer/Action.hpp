#include <iostream>
#include <string>
#include <sstream>

#include <ClientServerStringReversal/Shared/data.h>

template<typename ServerData>
struct Action
{
	ServerData operator()(data const& d) const
	{
		double x = d.real * d.integer;

		ServerData result;

		std::ostringstream oss;
		oss << "Result for request id: " << d.id << " is " << x << std::endl;
		result = oss.str();

		return result;
	}
};