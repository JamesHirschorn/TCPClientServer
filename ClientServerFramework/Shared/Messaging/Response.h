/* Struct for server responses. */

#include <string>

namespace ClientServer {

	enum TransmitionStatus {OK, BAD};

	struct Response
	{
		TransmitionStatus	transmission_status;
		std::string			message;
	};

}	// namespace TCPClientServer