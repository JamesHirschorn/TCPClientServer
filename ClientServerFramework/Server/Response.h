/* Struct for server responses. */

#ifndef CLIENT_SERVER_RESPONSE_H
#define CLIENT_SERVER_RESPONSE_H

#include <string>

namespace ClientServer {

	enum TransmitionStatus {OK, BAD};

	struct Response
	{
		TransmitionStatus	transmission_status;
		std::string			message;
	};

}	// namespace TCPClientServer

#endif	// !CLIENT_SERVER_RESPONSE_H