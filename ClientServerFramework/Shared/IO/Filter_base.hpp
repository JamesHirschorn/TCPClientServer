/**
*	Interface for the Stategy pattern for IO filtering.
*/

#ifndef FRAMEWORK_SHARED_IO_FILTER_BASE_HPP
#define FRAMEWORK_SHARED_IO_FILTER_BASE_HPP

#include <string>
#include <vector>

namespace io {

	class Filter_base
	{
	public:	
		/// output filter
		virtual void output_filter(std::string const& outbound, std::string& filtered) const = 0;

		/// input filter
		virtual void input_filter(std::vector<char> const& inbound, std::string& filtered) const = 0;
	};

}

#endif	// !FRAMEWORK_SHARED_IO_FILTER_BASE_HPP