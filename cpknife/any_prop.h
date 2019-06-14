#pragma once

#include <functional>
#include <string>
#include <map>
#include <memory>
#include <typeinfo>
#include "cpknife_exports.h"

namespace cpknife 
{
	class CPKNIFE_API any_prop {

		struct PropertyBase {
			PropertyBase(const type_info* a_type) :
				m_type(a_type) {};
			virtual             ~PropertyBase() {};

			const type_info*    m_type;
		};

		template< class T >
		struct Property : public PropertyBase {
			Property(const T& a_data) :
				PropertyBase(&typeid(T)),
				m_data(a_data) {};
			~Property() {};

			T                   m_data;
		};

	public:
		template< class T >
		T*                  Set(const std::string&, const T&);

		template< class T >
		T*                  Get(const std::string&);

	public:
		typedef std::map<std::size_t,
			std::pair<std::shared_ptr<PropertyBase>, std::string>> property_value;

	private:

		property_value m_properties;

	public:
		std::string PropertiesInfo();
	};


	// ----------------------------------------------------------------------------
	template< class T >
	T* any_prop::Set(const std::string& a_key, const T& a_data) {
		std::size_t hash = std::hash< std::string >()(a_key);
		m_properties[hash] = std::make_pair(
			std::shared_ptr< PropertyBase >(new Property< T >(a_data)),
			a_key);

		return &static_cast<Property< T >*>(m_properties[hash].first.get())->m_data;
	};

	// ----------------------------------------------------------------------------
	template < class T >
	T* any_prop::Get(const std::string& a_key) {
		std::size_t hash = std::hash< std::string >()(a_key);
		auto result = m_properties.find(hash);

		if (&typeid(T) == result->second.first.get()->m_type) {
			if (result == m_properties.end()) {
				return nullptr;
			}
			else {
				return &static_cast<Property< T >*>(result->second.first.get())->m_data;
			};
		};

		return nullptr;
	}
}
