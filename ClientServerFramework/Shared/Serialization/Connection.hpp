/** 
 *	Concrete implementation of Connection_base<InternetProtocol>.
 */

#ifndef FRAMEWORK_SHARED_SERIALIZATION_CONNECTION_HPP
#define FRAMEWORK_SHARED_SERIALIZATION_CONNECTION_HPP

#include <ClientServerFramework/Shared/Serialization/Connection_base.hpp>
#include <ClientServerFramework/Shared/Serialization/UnsecuredIO.hpp>

namespace io {

	template<typename InternetProtocol>
	class Connection :
		public Connection_base<InternetProtocol>
	{
		typedef typename internet_protocol::socket socket_type;
		UnsecuredIO io_;
	public:
		/// ctor
		Connection(boost::asio::io_service& io_service) :
			Connection_base(io_),
			io_(io_service)
		{}


		/// socket inspector
	/*	socket_type& socket()
		{
			return socket_;
		}*/
	private:

		///* implementation of abstract methods */

		//void async_write_impl(
		//	std::vector<boost::asio::const_buffer> const& buffers,
		//	async_handler const& handler)
		//{
		//	boost::asio::async_write(socket_, buffers, handler);
		//}

		//void async_read_impl(
		//	input_header_type& input,
		//	async_handler const& handler)
		//{
		//	boost::asio::async_read(socket_, boost::asio::buffer(input), handler);
		//}

		//void async_read_impl(
		//	std::vector<char>& input,
		//	async_handler const& handler)
		//{
		//	boost::asio::async_read(socket_, boost::asio::buffer(input), handler);
		//}

		//std::size_t write_impl(
		//	std::vector<boost::asio::const_buffer> const& buffers,
		//	boost::system::error_code& ec)
		//{
		//	return boost::asio::write(socket_, buffers, boost::asio::transfer_all(), ec);
		//}

		//std::size_t read_impl(
		//	input_header_type& input,
		//	boost::system::error_code& ec)
		//{
		//	return boost::asio::read(socket_, boost::asio::buffer(input), ec);
		//}

		//std::size_t read_impl(
		//	std::vector<char>& b, 
		//	boost::system::error_code& ec)
		//{
		//	return boost::asio::read(socket_, boost::asio::buffer(b), ec);
		//}*/

		/// Since the hooks are being used in conjunction with the decorator pattern, 
		/// the concrete decorator classes will also need to be able to access them.
		template<typename Protocol>
		friend class ServerConnection;
	};

}	// namespace io

#endif	// !FRAMEWORK_SHARED_SERIALIZATION_CONNECTION_HPP