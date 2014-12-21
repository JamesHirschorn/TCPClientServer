#include "client_data.hpp"

std::ostream& operator<<(std::ostream& os, client_data::operation_type operation)
{
	return os << static_cast<std::string>(operation);
}
