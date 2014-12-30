#include <ClientServerFramework/Shared/IO/Filter_base.hpp>
#include <ClientServerFramework/Shared/IO/DefaultFilter.hpp>
#include <ClientServerFramework/Shared/IO/CompressionFilter.hpp>

namespace io {

	class Filter_base_factory
	{
	public:
		static Filter_base* create(bool compressed)
		{
			Filter_base* filter;

			if (compressed)
			{
				filter = new CompressionFilter();
			}
			else
				filter = new DefaultFilter();

			return filter;
		}
	};

}