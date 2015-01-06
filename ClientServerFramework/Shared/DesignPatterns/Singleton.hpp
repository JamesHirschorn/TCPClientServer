/* A simple thread-safe generic indestructible Singleton. */

#ifndef FRAMEWORK_SHARED_DESIGNPATTERNS_SINGLETON_HPP
#define FRAMEWORK_SHARED_DESIGNPATTERNS_SINGLETON_HPP

namespace patterns {

	template<typename T>
	class Singleton
	{
	public:
		// returns a reference to the single instance of type T
		static T& get_instance()
		{
			// This is thread-safe in C++11.
			static T instance;

			return instance;
		}
	private:
		~Singleton() {}
	};

}	// namespace patterns

#endif	// !FRAMEWORK_SHARED_DESIGNPATTERNS_SINGLETON_HPP