/** Thread-safe queue class. */

#include <mutex>
#include <queue>

namespace safe_container {

	template <typename T, typename Container = std::deque<T>>
	class queue
	{
		mutable std::mutex _lock;
		typedef std::queue<T, Container> queue_type;
		queue_type queue_impl_;
	public:
		typedef typename queue_type::reference reference;
		typedef typename queue_type::const_reference const_reference;

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
		bool empty() const
		{
			std::lock_guard<std::mutex> lk(_lock);
			return queue_impl_.empty();
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