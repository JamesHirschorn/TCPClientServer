/** Forward declarations for IO_base.hpp. */

#ifndef FRAMEWORK_SHARED_SERIALIZATION_IO_BASE_FWD_HPP
#define FRAMEWORK_SHARED_SERIALIZATION_IO_BASE_FWD_HPP

#define define_type(t) typedef typename IO_base<InternetProtocol>::t t

namespace io {

	/*template<typename InternetProtocol, std::size_t header_length = 8>
	struct IO_base_typedefs
	{
		typedef InternetProtocol internet_protocol;
		typedef typename internet_protocol::resolver resolver_type;
		typedef typename resolver_type::query query_type;
		typedef typename resolver_type::iterator endpoint_iterator;
		typedef typename internet_protocol::acceptor acceptor_type;
		typedef std::function<void(boost::system::error_code const&, std::size_t)> async_handler;
		typedef std::function<void(boost::system::error_code const&)> initialize_handler;
		typedef std::function<void(boost::system::error_code const&)> async_handshake_handler;
		typedef std::function<void(boost::system::error_code const&)> accept_handler;
		typedef std::array<char, header_length> inbound_header_type;

		enum side { CLIENT, SERVER };
	};*/

	template<typename InternetProtocol, std::size_t header_length = 8>
	class IO_base;	// fwd declaration
}

#endif	// !FRAMEWORK_SHARED_SERIALIZATION_IO_BASE_FWD_HPP