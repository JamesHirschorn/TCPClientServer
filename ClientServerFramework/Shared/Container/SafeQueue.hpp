/** Thread-safe queue class. */

#include <mutex>
#include <queue>

namespace safe_container {

	template <typename T, typename Container = std::deque<T>>
	class queue
	{
		// To simplify use cases:
		static_assert(std::is_nothrow_copy_constructible<T>::value, "whoops - not desired");

		mutable std::mutex _lock;
		std::queue<T, Container> queue_impl_;
	public:
		typedef queue_impl_::reference reference;
		typedef queue_impl_::const_reference const_reference;

		reference back()
		{
			std::lock_guard<std::mutex> lk(_lock);
			return queue_impl_.back();
		}
		const_reference back() const
		{
			std::lock_guard<std::mutex> lk(_lock);
			return queue_impl_.back();
		}
		reference front()
		{
			std::lock_guard<std::mutex> lk(_lock);
			return queue_impl_.front();
		}
		const_reference front() const
		{
			std::lock_guard<std::mutex> lk(_lock);
			return queue_impl_.front();
		}
		void pop()
		{
			std::lock_guard<std::mutex> lk(_lock);
			queue_impl_.pop();
		}

		void push(T const& element)
		{
			std::lock_guard<std::mutex> lk(_lock);
			queue_impl_.push(element);
		}
	};


}	// namespace safe_container