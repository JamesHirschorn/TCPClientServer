#include <algorithm>
#include <string>

struct Action
{
	template<typename T>
	void operator()(T& data) const
	{
		using namespace std;

		T temp;
		temp.resize(data.size());

		copy(data.rbegin(), data.rend(), temp.begin());
		copy(temp.begin(), temp.end(), data.begin());
	}
};