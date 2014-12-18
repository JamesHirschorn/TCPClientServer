#include <istream>
#include <queue>

#include <Singleton.hpp>

namespace TCPClientServer
{

	template<typename Data, typename Queue = template std::queue<typename>>
	class Scraper
	{
	protected:
		// ctor
		Scraper(std::istream& is)
			: is_(is), queue(patterns::Singleton<Queue>.get_instance())
		{}
	public:
		virtual void start() = 0;

		// dtor
		virtual ~Scraper() {}
	private:
		istream& is_;
		Queue& queue;
	};

}

