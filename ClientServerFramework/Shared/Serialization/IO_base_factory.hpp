#include <ClientServerFramework/Shared/Serialization/IO_base.hpp>
#include <ClientServerFramework/Shared/Serialization/SSL_IO.hpp>
#include <ClientServerFramework/Shared/Serialization/UnsecuredIO.hpp>

namespace io {

	template<typename InternetProtocol>
	class IO_base_factory
	{
	public:
		typedef IO_base<InternetProtocol> IO_base_type;
		typedef typename IO_base_type::internet_protocol internet_protocol;

		static IO_base_type* create(
			boost::asio::io_service& io_service,
			io::ssl_options const& SSL_options,
			typename IO_base_type::side s)
		{
			IO_base_type* io;

			switch (SSL_options.mode)
			{
			case OFF:
				io = new UnsecuredIO<internet_protocol>(io_service);
				break;
			case SSLV23:
			case SSLV3:
				io = new SSL_IO<internet_protocol>(io_service, SSL_options, s);
				break;
			default:
				throw std::runtime_error("Invalid SSL mode.");
			}

			return io;
		}
	};

}