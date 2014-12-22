#include <ClientServerFramework/Shared/Serialization/Connection_base.hpp>

namespace io {

	template<typename Socket>
	class Connection :
		public Connection_base
	{
	public:
		typedef Socket socket_type;

		Connection(boost::asio::io_service& io_service) :
			Connection_base(io_service, impl)
		{}
	private:
		/// The underlying socket.
		socket_type socket_;


	};

}