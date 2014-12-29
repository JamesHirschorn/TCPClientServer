/**
*	Interface for the Stategy pattern for socket IO.
*/

#ifndef FRAMEWORK_SHARED_IO_SERIALIZATION_BASE_HPP
#define FRAMEWORK_SHARED_IO_SERIALIZATION_BASE_HPP

#include <ClientServerFramework/Shared/IO/SSL.hpp>

#include <boost/archive/polymorphic_iarchive.hpp>
#include <boost/archive/polymorphic_oarchive.hpp>

#include <boost/archive/polymorphic_text_iarchive.hpp>
#include <boost/archive/polymorphic_text_oarchive.hpp>

#include <array>
#include <functional>
#include <vector>

namespace io {

	template<typename InternetProtocol>
	class Serialization_base
	{
	public:
		template<typename T>
		void instantiate_iarchive(T& t)
		{

			return boost::archive::polymorphic_text_iarchive(t);
		}
		template<typename T>
		boost::archive::polymorphic_oarchive create_oarchive(T& t)
		{
			return boost::archive::polymorphic_text_oarchive(t);
		}
		boost::archive::polymorphic_iarchive* iarchive;
		boost::archive::polymorphic_oarchive* oarchive;
	private:
		virtual boost::archive::polymorphic_iarchive* create_iarchive
	};

}

#endif	// !FRAMEWORK_SHARED_IO_SERIALIZATION_BASE_HPP