/**
 * 1. Added synchronzied read and write to the boost::asio example.
 * 2. Clean up async_read and async_write to work with lambdas.
 *
 * (C) James Hirschorn 2014
 */

//
// Connection.hpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2014 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef SERIALIZATION_CONNECTION_HPP
#define SERIALIZATION_CONNECTION_HPP

#include <boost/asio.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>

namespace ClientServer {

	/// The connection class provides serialization primitives on top of a socket.
	/**
	* Each message sent using this class consists of:
	* @li An 8-byte header containing the length of the serialized data in
	* hexadecimal.
	* @li The serialized data.
	*/
	class Connection
	{
	public:
		/// Constructor.
		Connection(boost::asio::io_service& io_service)
			: socket_(io_service)
		{
		}

		/// Get the underlying socket. Used for making a Connection or for accepting
		/// an incoming connection.
		boost::asio::ip::tcp::socket& socket()
		{
			return socket_;
		}

		/// Asynchronously write a data structure to the socket.
		template <typename T, typename Handler>
		void async_write(const T& t, Handler handler)
		{
			// Serialize the data first so we know how large it is.
			std::ostringstream archive_stream;
			boost::archive::text_oarchive archive(archive_stream);
			archive << t;
			outbound_data_ = archive_stream.str();

			// Format the header.
			std::ostringstream header_stream;
			header_stream << std::setw(header_length)
				<< std::hex << outbound_data_.size();
			if (!header_stream || header_stream.str().size() != header_length)
			{
				// Something went wrong, inform the caller.
				handler(boost::asio::error::invalid_argument, 0);
				//boost::system::error_code error(boost::asio::error::invalid_argument);
				//socket_.get_io_service().post(boost::bind(handler, error));
				return;
				//throw boost::system::system_error(error);
			}
			outbound_header_ = header_stream.str();

			// Write the serialized data to the socket. We use "gather-write" to send
			// both the header and the data in a single write operation.
			std::vector<boost::asio::const_buffer> buffers;
			buffers.push_back(boost::asio::buffer(outbound_header_));
			buffers.push_back(boost::asio::buffer(outbound_data_));
			boost::asio::async_write(socket_, buffers, handler);
		}

		template<typename T>
		void write(T const& t, boost::system::error_code& ec)
		{
			// Serialize the data first so we know how large it is.
			std::ostringstream archive_stream;
			boost::archive::text_oarchive archive(archive_stream);
			archive << t;
			outbound_data_ = archive_stream.str();

			// Format the header.
			std::ostringstream header_stream;
			header_stream << std::setw(header_length)
				<< std::hex << outbound_data_.size();
			if (!header_stream || header_stream.str().size() != header_length)
			{
				// Something went wrong, inform the caller.
				boost::system::error_code error(boost::asio::error::invalid_argument);
				throw boost::system::system_error(error);
				//socket_.get_io_service().post(boost::bind(handler, error));
				//return;
			}
			outbound_header_ = header_stream.str();

			// Write the serialized data to the socket. We use "gather-write" to send
			// both the header and the data in a single write operation.
			std::vector<boost::asio::const_buffer> buffers;
			buffers.push_back(boost::asio::buffer(outbound_header_));
			buffers.push_back(boost::asio::buffer(outbound_data_));

			boost::asio::write(socket_, buffers, ec);
		}

		/// Asynchronously read a data structure from the socket.
		template <typename T, typename Handler>
		void async_read(T& t, Handler const& handler)
		{
			// Issue a read operation to read exactly the number of bytes in a header.
			/*void (Connection::*f)(
				const boost::system::error_code&,
				T&, boost::tuple<Handler>)
				= &Connection::handle_read_header<T, Handler>;*/
			boost::asio::async_read(socket_, boost::asio::buffer(inbound_header_), 
				[this,&t,handler](boost::system::error_code const& ec, std::size_t len)
			{
				handle_read_header(ec, len, t, handler);
			});
				/*boost::bind(f,
				this, boost::asio::placeholders::error, boost::ref(t),
				boost::make_tuple(handler)));*/
		}

		/// Handle a completed read of a message header. The handler is passed using
		/// a tuple since boost::bind seems to have trouble binding a function object
		/// created using boost::bind as a parameter.
		template <typename T, typename Handler>
		void handle_read_header(const boost::system::error_code& e, std::size_t len,
			T& t, Handler const& handler)
		{
			if (e)
			{
				handler(e, len);
				//boost::get<0>(handler)(e);
				//handler(e, 0);
			}
			else
			{
				// Determine the length of the serialized data.
				std::istringstream is(std::string(inbound_header_, header_length));
				std::size_t inbound_data_size = 0;
				if (!(is >> std::hex >> inbound_data_size))
				{
					// Header doesn't seem to be valid. Inform the caller.
					boost::system::error_code error(boost::asio::error::invalid_argument);
					throw boost::system::system_error(error);
					//boost::get<0>(handler)(error);
					//return;
				}

				// Start an asynchronous call to receive the data.
				inbound_data_.resize(inbound_data_size);
				/*void (Connection::*f)(
					const boost::system::error_code&,
					T&, boost::tuple<Handler>)
					= &Connection::handle_read_data<T, Handler>;*/
				boost::asio::async_read(socket_, boost::asio::buffer(inbound_data_),
					[this, &t, handler](boost::system::error_code const& ec, std::size_t len)
				{
					handle_read_data(ec, len, t, handler);
				});
				/* boost::bind(f, this,
					boost::asio::placeholders::error, boost::ref(t), handler)); */
			}
		}

		/// Handle a completed read of message data.
		template <typename T, typename Handler>
		void handle_read_data(const boost::system::error_code& ec, std::size_t len,
			T& t, Handler const& handler)
		{
			if (ec)
			{
				handler(ec, len);
			}
			else
			{
				// Extract the data structure from the data just received.
				try
				{
					std::string archive_data(&inbound_data_[0], inbound_data_.size());
					std::istringstream archive_stream(archive_data);
					boost::archive::text_iarchive archive(archive_stream);
					archive >> t;
				}
				catch (std::exception& e)
				{
					throw e;
				}

				// Inform caller that data has been received ok.
				//boost::get<0>(handler)(e);
				handler(ec, len);
			}
		}
		/// Synchronously read the data from the socket.
		template<typename T>
		void read(T& t, boost::system::error_code& ec)
		{
			using namespace boost::asio;

			boost::asio::read(socket_, buffer(inbound_header_), ec);

			if (ec) return;	// an error occurred.

			// Determine the length of the serialized data.
			std::string s(inbound_header_, header_length);
			std::istringstream is(std::string(inbound_header_, header_length));
			std::size_t inbound_data_size = 0;
			if (!(is >> std::hex >> inbound_data_size))
			{
				// Header doesn't seem to be valid. Inform the caller.
				ec = boost::system::error_code(boost::asio::error::invalid_argument);
				return;
			}

			inbound_data_.resize(inbound_data_size);
			boost::asio::read(socket_, buffer(inbound_data_), ec);

			// Extract the data structure from the data just received.
			try
			{
				std::string archive_data(&inbound_data_[0], inbound_data_.size());
				std::istringstream archive_stream(archive_data);
				boost::archive::text_iarchive archive(archive_stream);
				archive >> t;
			}
			catch (std::exception& e)
			{
				// Unable to decode data.
				ec = boost::system::error_code(boost::asio::error::invalid_argument);
				throw e;
			}
		}
	private:
		/// The underlying socket.
		boost::asio::ip::tcp::socket socket_;

		/// The size of a fixed length header.
		enum { header_length = 8 };

		/// Holds an outbound header.
		std::string outbound_header_;

		/// Holds the outbound data.
		std::string outbound_data_;

		/// Holds an inbound header.
		char inbound_header_[header_length];

		/// Holds the inbound data.
		std::vector<char> inbound_data_;
	};

} // namespace s11n_example

#endif // SERIALIZATION_CONNECTION_HPP