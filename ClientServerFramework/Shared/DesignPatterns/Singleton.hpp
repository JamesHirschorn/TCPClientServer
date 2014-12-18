/* A simple thread-safe generic indestructible Singleton. */

namespace patterns {

	template<typename T>
	class Singleton
	{
	public:
		// returns a reference to the single instance of type T
		T& get_instance()
		{
			// This is thread-safe in C++11.
			static T instance;

			return instance;
		}
	private:
		~Singleton() {}
	};

}	// namespace patterns