#include <iomanip>

#include "client_data.hpp"

std::ostream& operator<<(std::ostream& os, client_data::operation_type operation)
{
	return os << static_cast<std::string>(operation);
}

std::ostream& operator<<(std::ostream& os, client_data data)
{
	return os /* Suppress the client id: << data.client_id << ' ' */ 
		<< data.id << ' '
		<< std::setprecision(15) << data.operand1 << ' ' << data.operation << ' ' 
		<< std::setprecision(15) << data.operand2
		<< std::endl;
}
