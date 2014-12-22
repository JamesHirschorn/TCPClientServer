#include <ClientServerFramework/Shared/Serialization/Connection_base.hpp>

namespace io {

	template<typename Socket>
	class Connection :
		public Connection_base
	{
	public:
		typedef Socket socket_type;

	private:
		/// The underlying socket.
		socket_type socket_;


	};

}