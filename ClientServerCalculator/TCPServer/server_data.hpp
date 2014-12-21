/** The serializable data structure sent by the server 
 *	in response to the client. 
 */

#ifndef CALCULATOR_TCPSERVER_SERVER_DATA_HPP
#define CALCULATOR_TCPSERVER_SERVER_DATA_HPP

#include <cstdint>
#include <string>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/time_serialize.hpp>

struct server_data
{
	boost::posix_time::ptime	timestamp;	// server timestamp
	int32_t						id;			// request id
	double						result;		// calculation result

	/// to satisfy serializable concept
	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & timestamp;
		ar & id;
		ar & result;
	}
};

#endif	// !CALCULATOR_TCPSERVER_SERVER_DATA_HPP