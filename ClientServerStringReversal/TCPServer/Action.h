#include <algorithm>
#include <string>

struct Action
{
	std::string operator()(std::string& data) const
	{
		using namespace std;

		string temp;

		copy(data.rbegin(), data.rend(), temp.begin());
		copy(temp.begin(), temp.end(), data.begin());
	}
};