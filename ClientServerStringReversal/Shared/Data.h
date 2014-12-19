/** A serializable data structure. */

#ifndef CLIENT_SERVER_DATA
#define CLIENT_SERVER_DATA

#include <cstdint>
#include <string>

struct data
{
	std::string	request;	// request
	int32_t		id;			// request id
	int64_t		integer;	// integer used in calculation
	double		real;		// real number used in calculation

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & integer;
		ar & real;
		ar & id;
	}
};

#endif	// !CLIENT_SERVER_DATA