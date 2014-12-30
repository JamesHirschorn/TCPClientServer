/**
*	Default concrete strategy for IO filtering. 
*	
*	Just does copy-through.
*/

#ifndef FRAMEWORK_SHARED_IO_DEFAULTFILTER_HPP
#define FRAMEWORK_SHARED_IO_DEFAULTFILTER_HPP

#include <ClientServerFramework/Shared/IO/Filter_base.hpp>


namespace io {

	class DefaultFilter : public
		Filter_base
	{
	public:
		/// Sends the output as is.
		void output_filter(std::string const& outbound, std::string& filtered) const
		{
			// move semantics for efficiency
			filtered = std::move(outbound);
		}

		/// Just creates a string copy of the data as is.
		void input_filter(std::vector<char> const& inbound, std::string& filtered) const
		{
			filtered = std::move(std::string(&inbound[0], inbound.size()));
		}
	};

}	// namespace io

#endif	// !FRAMEWORK_SHARED_IO_DEFAULTFILTER_HPP