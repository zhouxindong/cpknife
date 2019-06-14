#pragma once

/*
** any_bag
** Reference by Pavan Kumar Sunkara, thanks you
** Adjust by zhouxindong
** used for add property step by step
** finally you can transform any_bag to json text
** any json parser can parse it and retrive property from parsed result
** further work: 
** 1. implement any_bag parser
** enjoy it yourself :)
*/

#include <string>
#include <vector>
#include <map>
#include <ostream>
#include <algorithm>
#include <memory>
#include <stdexcept>
#include <sstream>
#include "cpknife_exports.h"

namespace cpknife {

	/** Forward Declaration */
	class CPKNIFE_API _base_value;

	/** Key-value pairs (object) */
	typedef std::map<std::string, _base_value> object_type;

	/** List of keys for object (for deterministic output) */
	typedef std::vector<std::string> object_key_type;

	/** Array of values */
	typedef std::vector<_base_value> array_type;

	/** Base class for values */
	class CPKNIFE_API _base_value {
	public:

		/** Enum for Type of the value */
		enum CPKNIFE_API _E_value_type {
			undefined_type_E = 0,
			null_type_E,     // Null
			string_type_E,   // A string
			number_type_E,   // A number
			bool_type_E,  // A boolean value
			array_type_E,    // An array
			object_type_E    // An object
		};

		/** Type of the value */
		_E_value_type type;

		/** String value */
		std::string str;

		/** Number value */
		double number;

		/** Boolean value */
		bool boolean;

		/** Object value */
		object_type& object();
		const object_type& object() const;

		object_key_type keys;

		/** Array value */
		array_type& array();
		const array_type& array() const;

		_base_value(_E_value_type type_ = undefined_type_E);

		_base_value(const _base_value& rhs);

		_base_value& operator=(const _base_value& rhs);

		~_base_value() {};

	private:
		std::unique_ptr<object_type> _pobj;
		std::unique_ptr<array_type> _pary;
	};

	class CPKNIFE_API BAG_null : public _base_value {
	public:

		BAG_null();
	};

	class CPKNIFE_API BAG_string : public _base_value {
	public:

		BAG_string(std::string string_ = "");
	};

	class CPKNIFE_API BAG_number : public _base_value {
	public:

		BAG_number(double number_ = 0);
	};

	class CPKNIFE_API BAG_bool : public _base_value {
	public:

		BAG_bool(bool boolean_ = false);
	};

	class CPKNIFE_API BAG_array : public _base_value {
	public:

		BAG_array();

		void push(const _base_value& value);
		void set(const size_t index, const _base_value& value);

		bool empty();
	};

	class CPKNIFE_API BAG_object : public _base_value {
	public:

		BAG_object();

		void set(const std::string& key, const _base_value& value, bool doNotOverride = false);
		void unset(const std::string& key);

		bool empty();
	};

	/** Base class for serializers */
	struct CPKNIFE_API serialize {

		/** Constructor */
		serialize();

		/** Destructor */
		virtual ~serialize() {};

		/** Process the given value with the serializer logic */
		void process(const _base_value& value, std::ostream& os, size_t level = 0);

		virtual void null_(std::ostream& os) = 0;
		virtual void string_(const std::string& value, std::ostream& os) = 0;
		virtual void number_(double value, std::ostream& os) = 0;
		virtual void bool_(bool value, std::ostream& os) = 0;
		virtual void array_(const _base_value& value, std::ostream& os, size_t level) = 0;
		virtual void object_(const _base_value& value, std::ostream& os, size_t level) = 0;

		virtual void indent_(size_t level, std::ostream& os) = 0;
	};

	struct CPKNIFE_API serialize_json : public serialize {

		void indent_(size_t level, std::ostream& os) override;

		void null_(std::ostream& os) override;

		void string_(const std::string& value, std::ostream& os) override;

		void number_(double value, std::ostream& os) override;

		void bool_(bool value, std::ostream& os) override;

		void array_(const _base_value& value, std::ostream& os, size_t level) override;

		void object_(const _base_value& value, std::ostream& os, size_t level) override;
	};

	/**
	*  \brief  Escape every backslash in input string.
	*  \param  input   A string to escape its backslashes.
	*  \return A new string with backslashes escaped.
	*/
	std::string escape_backslashes(const std::string& input);

	/**
	*  \brief  Escape every double quote in input string.
	*  \param  input   A string to escape its double quotes.
	*  \return A new string with double quotes escaped.
	*/
	std::string escape_double_quotes(const std::string& input);

	/**
	*  \brief  Escape new lines in input string.
	*  \param  input   A string to escape its new lines.
	*  \return A new string with lines escaped.
	*
	*  This function replaces all occurences of "\n" with "\\n".
	*/
	std::string escape_newlines(const std::string& input);
}