/** A serializable data structure. */

#ifndef CLIENT_SERVER_DATA
#define CLIENT_SERVER_DATA

#include <string>

struct data
{
	int64_t		integer;
	double		real;
	std::string id;

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & integer;
		ar & real;
		ar & id;
	}
};

#endif	// !CLIENT_SERVER_DATA