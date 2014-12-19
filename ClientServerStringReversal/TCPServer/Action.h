#include <algorithm>
#include <string>

struct Action
{
	template<typename Data>
	void operator()(Data& data) const
	{
		using namespace std;

		Data temp;
		temp.resize(data.size());

		copy(data.rbegin(), data.rend(), temp.begin());
		copy(temp.begin(), temp.end(), data.begin());
	}
};