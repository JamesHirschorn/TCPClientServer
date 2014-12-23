/**
 * Add serialization capabilities to a socket.
 * This can only be used as a base class.
 * Based on the boost::asio serialization example "Connection.hpp".
 *
 * 1. Added synchronzied read and write to Connection.hpp.
 * 2. Major clean up, so that now async_read and async_write work with lambdas.
 * 3. Made the Connection_base class polymorphic so that we can also serialize
 *	  encrypted/compressed socket connections.
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

#ifndef FRAMEWORK_SERIALIZATION_CONNECTION_HPP
#define FRAMEWORK_SERIALIZATION_CONNECTION_HPP

#include <ClientServerFramework/Shared/Serialization/Connection_impl.hpp>

#include <boost/asio/write.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/system/error_code.hpp>
#include <boost/tuple/tuple.hpp>

#include <iomanip>
#include <functional>
#include <string>
#include <sstream>
#include <vector>

namespace io {

	/// The connection class provides serialization primitives on top of a socket.
	/// This is an abstract base class.
	/**
	* Each message sent using this class consists of:
	* @li An 8-byte header containing the length of the serialized data in
	* hexadecimal.
	* @li The serialized data.
	*/
	template<typename InternetProtocol>
	class Connection_base
	{
		typedef typename InternetProtocol::resolver resolver_type;
	protected:
		typedef std::function<void(boost::system::error_code const&, std::size_t)> async_handler_type;
	public:
		//typedef typename resolver_type::query query_type;
		typedef typename resolver_type::iterator endpoint_iterator_type;
		/// Get the underlying socket. Used for making a Connection or for accepting
		/// an incoming connection.
	/*	socket_type& socket()
		{
			return socket_;
		}*/

		/// Connect to the underlying socket.
		virtual endpoint_iterator_type connect(
			endpoint_iterator_type begin, 
			boost::system::error_code& ec) = 0;
		

		/// Asynchronously write a data structure to the socket.
		template <typename T>
		void async_write(
			T const& t, 
			async_handler_type const& handler)
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
				handler(boost::asio::error::invalid_argument, header_length);
				//boost::system::error_code error(boost::asio::error::invalid_argument);
				//socket_.get_io_service().post(boost::bind(handler, error));
				return;
			}
			outbound_header_ = header_stream.str();

			// Write the serialized data to the socket. We use "gather-write" to send
			// both the header and the data in a single write operation.
			std::vector<boost::asio::const_buffer> buffers;
			buffers.push_back(boost::asio::buffer(outbound_header_));
			buffers.push_back(boost::asio::buffer(outbound_data_));
			//boost::asio::async_write(socket_, buffers, handler);
			async_write_impl(buffers, handler);
		}

		/// Synchronized write. 
		/// Return value indicates the number of bytes written.
		template<typename T>
		std::size_t write(T const& t, boost::system::error_code& ec)
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
				ec = boost::asio::error::invalid_argument;
				//socket_.get_io_service().post(boost::bind(handler, error));
				return header_stream.str().size();
			}
			outbound_header_ = header_stream.str();

			// Write the serialized data to the socket. We use "gather-write" to send
			// both the header and the data in a single write operation.
			std::vector<boost::asio::const_buffer> buffers;
			buffers.push_back(boost::asio::buffer(outbound_header_));
			buffers.push_back(boost::asio::buffer(outbound_data_));

			return write_impl(buffers, ec);
		}

		/// Asynchronously read a data structure from the socket.
		template <typename T>
		void async_read(T& t, async_handler_type const& handler)
		{
			asyc_read_impl(inbound_header_, 
				[this,&t,handler](boost::system::error_code const& ec, std::size_t len)
			{
				handle_read_header(ec, len, t, handler);
			});
		}

		/// Handle a completed read of a message header. 
		template <typename T>
		void handle_read_header(boost::system::error_code const& e, std::size_t len,
			T& t, async_handler_type const& handler)
		{
			if (e)
			{
				handler(e, len);
			}
			else
			{
				// Determine the length of the serialized data.
				std::istringstream is(std::string(inbound_header_, header_length));
				std::size_t inbound_data_size = 0;
				if (!(is >> std::hex >> inbound_data_size))
				{
					// Header doesn't seem to be valid. Inform the caller.
					handler(boost::asio::error::invalid_argument, len);
					return;
				}

				// Start an asynchronous call to receive the data.
				inbound_data_.resize(inbound_data_size);
				async_read_impl(inbound_data_
					[this,&t,len,handler](boost::system::error_code const& ec, std::size_t next_len)
				{
					handle_read_data(ec, len + next_len, t, handler);
				});
			}
		}

		/// Handle a completed read of message data.
		template <typename T>
		void handle_read_data(const boost::system::error_code& ec, std::size_t len,
			T& t, async_handler_type const& handler)
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
				handler(ec, len);
			}
		}
		
		/// Synchronously read the data from the socket.
		/// Return value indicates the number of bytes read.
		template<typename T>
		std::size_t read(T& t, boost::system::error_code& ec)
		{
			using namespace boost::asio;

			std::size_t length;

			//length = boost::asio::read(socket_, buffer(inbound_header_), ec);
			length = read(inbound_header_, ec);

			if (ec) 
				return length;	// an error occurred.

			// Determine the length of the serialized data.
			std::string s(inbound_header_, header_length);
			std::istringstream is(std::string(inbound_header_, header_length));
			std::size_t inbound_data_size = 0;
			if (!(is >> std::hex >> inbound_data_size))
			{
				// Header doesn't seem to be valid. Inform the caller.
				ec = boost::asio::error::invalid_argument;
				return 0;
			}

			inbound_data_.resize(inbound_data_size);
			//length += boost::asio::read(socket_, buffer(inbound_data_), ec);
			length += read_impl(inbound_data_, ec);

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

			return length;
		}
		/// dtor
		virtual ~Connection_base() 
		{}
	protected:
		/// Constructor.
	/*	Connection_base(boost::asio::io_service& io_service)
			: impl_(impl)
		{
		}*/
	private:
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

		virtual void async_write_impl(
			std::vector<boost::asio::const_buffer> const&, 
			async_handler_type const&) = 0;

		virtual void async_read_impl(
			std::vector<char>&,
			async_handler_type const&) = 0;

		virtual std::size_t write_impl(
			std::vector<boost::asio::const_buffer> const& buffers,
			boost::system::error_code& ec) = 0;

		virtual std::size_t read_impl(
			std::vector<char>& b,
			boost::system::error_code& ec) = 0;
	};

}	// namespace io

#endif // !FRAMEWORK_SERIALIZATION_CONNECTION_HPP