/* Struct for server responses. */

#ifndef FRAMEWORK_SERVER_RESPONSE_HPP
#define FRAMEWORK_SERVER_RESPONSE_HPP

#include <string>

namespace Server {

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

}	// namespace Server

#endif	// !FRAMEWORK_SERVER_RESPONSE_H