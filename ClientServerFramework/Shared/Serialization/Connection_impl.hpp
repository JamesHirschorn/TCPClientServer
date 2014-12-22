/** The implementation for Connection_base.
 *
 *	This is essehtailly an application of the Pimpl Idiom. 
 *	However, it appears as a reference in Connection_base 
 * (rather than a pointer) because Connection_base 
 * is a component of the Connector class, where the implementation is chosen.
 */

namespace io { 
namespace detail {

	struct Connection_impl
	{
		template<
			typename Iterator>
			Iterator connect(
			Iterator begin,
			boost::system::error_code & ec);

		template<
			typename ConstBufferSequence,
			typename WriteHandler>
			void async_write_impl(
			ConstBufferSequence const& buffers,
			WriteHandler handler);

		template<
			typename ConstBufferSequence,
			typename CompletionCondition>
			std::size_t write_impl(
			ConstBufferSequence const& buffers,
			CompletionCondition completion_condition,
			boost::system::error_code& ec);

		template<
			typename MutableBufferSequence,
			typename ReadHandler>
			void async_read_impl(
			MutableBufferSequence const& buffers,
			ReadHandler handler);

		template<
			typename MutableBufferSequence>
			std::size_t read_impl(
			MutableBufferSequence const& buffers,
			boost::system::error_code & ec);
	};

}}	// namespace io::detail
