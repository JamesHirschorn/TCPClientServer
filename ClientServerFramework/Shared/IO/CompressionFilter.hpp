/**
*	Concrete strategy for IO compression filtering.
*
*	Uses zlib compression.
*/

#ifndef FRAMEWORK_SHARED_IO_COMPRESSIONFILTER_HPP
#define FRAMEWORK_SHARED_IO_COMPRESSIONFILTER_HPP
#pragma warning(disable:4244)

#include <ClientServerFramework/Shared/IO/Filter_base.hpp>

#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/zlib.hpp>

#include <iostream>

namespace io {

	class CompressionFilter : public
		Filter_base
	{
	public:
		/// Compression.
		void output_filter(std::string const& outbound, std::string& filtered) const
		{
			using namespace boost::iostreams;
			using namespace std;

			clog << "Size before compression: " << outbound.size() << endl;

			filtered.clear();
			filtering_ostream out;
			out.push(zlib_compressor());
			out.push(boost::iostreams::back_inserter(filtered));

			out << outbound;
		}

		/// Decompression.
		void input_filter(std::vector<char> const& inbound, std::string& filtered) const
		{
			using namespace boost::iostreams;

			//std::istringstream iss(inbound, std::ios_base::binary); 
			array_source source(&inbound[0], inbound.size());
			filtered.clear();
			filtering_ostream out(boost::iostreams::back_inserter(filtered));

			filtering_istream in;
			in.push(zlib_decompressor());
			in.push(source);
			boost::iostreams::copy(in, out);
		}
	};

}	// namespace io

#pragma warning(default:4244)
#endif	// !FRAMEWORK_SHARED_IO_COMPRESSIONFILTER_HPP