#include <memory>

#include <boost/asio/buffer.hpp>

namespace ClientServer {
	
	/// Note: enabled_shared_from_this is used to ensure that the session remains alive
	///		  as long as some operation refers to it.
	template<typename InternetProtocol>
	class Session :
		public std::enable_shared_from_this<Session>
	{
	public:
		/// ctor: Session`s are created using move semantics.
		Session(socket_type&& socket) : socket_(socket)
		{}

		void start()
		{
			do_receive();
		}
	private:
		void do_receive()
		{
			using boost::asio;

			auto self(shared_from_this());

			// for initial testing just read a string message, of maximum length 20
			static size_t const max_read_size = 20;
			std::array<char, max_read_size> a;
			

			socket_.async_read(buffer(a), 
				[this, self, a](boost::system::error_code const& ec, std::size_t length)
			{
				//action_(a);
			}
		}

		typedef typename InternetProtocol::socket socket_type;
		socket_type socket_;
	};

}