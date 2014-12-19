/* Struct for server responses. */

#ifndef CLIENT_SERVER_RESPONSE_H
#define CLIENT_SERVER_RESPONSE_H

#include <string>

namespace ClientServer {

	enum TransmitionStatus {OK, BAD};

	template<typename Data>
	struct Response
	{
		TransmitionStatus	transmission_status;
		Data				data;
	};

}	// namespace TCPClientServer

#endif	// !CLIENT_SERVER_RESPONSE_H