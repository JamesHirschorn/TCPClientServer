/* Struct for server responses. */

#ifndef CLIENT_SERVER_RESPONSE_H
#define CLIENT_SERVER_RESPONSE_H

#include <string>

namespace ClientServer {

	enum TransmitionStatus {OK, BAD};

	/// This is the type returned by the server.
	/// Data can be any serializable type.
	template<typename Data>
	struct Response
	{
		TransmitionStatus	transmission_status;
		Data				data;

		template <typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar & transmission_status;
			ar & data;
		}
	};

}	// namespace TCPClientServer

#endif	// !CLIENT_SERVER_RESPONSE_H