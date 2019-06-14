#include "stdafx.h"
#include "any_bag.h"

cpknife::_base_value::_base_value(_base_value::_E_value_type type_)
	: type(type_)
{
	_pobj.reset(::new object_type);
	_pary.reset(::new array_type);
}

cpknife::_base_value::_base_value(const cpknife::_base_value& rhs)
{
	this->type = rhs.type;
	this->str = rhs.str;
	this->number = rhs.number;
	this->boolean = rhs.boolean;
	this->keys = rhs.keys;

	this->_pobj.reset(::new object_type(*rhs._pobj.get()));
	this->_pary.reset(::new array_type(*rhs._pary.get()));
}

cpknife::_base_value& cpknife::_base_value::operator=(const cpknife::_base_value &rhs)
{
	this->type = rhs.type;
	this->str = rhs.str;
	this->number = rhs.number;
	this->boolean = rhs.boolean;
	this->keys = rhs.keys;

	this->_pobj.reset(::new object_type(*rhs._pobj.get()));
	this->_pary.reset(::new array_type(*rhs._pary.get()));

	return *this;
}

cpknife::object_type& cpknife::_base_value::object()
{
	if (!_pobj.get())
		throw std::logic_error("no object key-values set");

	return *_pobj;
}

const cpknife::object_type& cpknife::_base_value::object() const
{
	if (!_pobj.get())
		throw std::logic_error("no object key-values set");

	return *_pobj;
}

cpknife::array_type& cpknife::_base_value::array()
{
	if (!_pary.get())
		throw std::logic_error("no array values set");

	return *_pary;
}

const cpknife::array_type& cpknife::_base_value::array() const
{
	if (!_pary.get())
		throw std::logic_error("no array values set");

	return *_pary;
}

cpknife::BAG_null::BAG_null()
	: _base_value(null_type_E)
{}

cpknife::BAG_string::BAG_string(std::string str_)
{
	type = string_type_E;
	str = str_;
}

cpknife::BAG_number::BAG_number(double number_)
{
	type = number_type_E;
	number = number_;
}

cpknife::BAG_bool::BAG_bool(bool boolean_)
{
	type = bool_type_E;
	boolean = boolean_;
}

cpknife::BAG_array::BAG_array()
	: _base_value(array_type_E)
{}

void cpknife::BAG_array::push(const cpknife::_base_value& value)
{
	array().push_back(value);
}

void cpknife::BAG_array::set(const size_t index, const cpknife::_base_value& value)
{
	if (array().size() <= index)
		throw std::logic_error("not enough array values set");

	array().at(index) = value;
}

bool cpknife::BAG_array::empty()
{
	return array().empty();
}

cpknife::BAG_object::BAG_object()
	: _base_value(object_type_E)
{}

void cpknife::BAG_object::set(const std::string& key, const cpknife::_base_value& value, bool doNotOverride)
{
	cpknife::object_key_type::iterator it = std::find(keys.begin(), keys.end(), key);

	if (it != keys.end()) {
		if (doNotOverride) {
			throw std::logic_error("key already present in the object");
		}
		else {
			keys.erase(it);
		}
	}

	keys.push_back(key);
	object().operator[](key) = value;
}

void cpknife::BAG_object::unset(const std::string &key)
{
	cpknife::object_key_type::iterator it = std::find(keys.begin(), keys.end(), key);

	if (it == keys.end()) {
		return;
	}

	cpknife::object_type::iterator valueIt = object().find(key);

	keys.erase(it);
	object().erase(valueIt);
}

bool cpknife::BAG_object::empty()
{
	return keys.empty();
}

cpknife::serialize::serialize()
{}

void cpknife::serialize::process(const _base_value& root, std::ostream& os, size_t level)
{
	cpknife::_base_value::_E_value_type type = root.type;

	switch (type) {

	case _base_value::null_type_E:
		null_(os);
		break;

	case _base_value::string_type_E:
		string_(root.str, os);
		break;

	case _base_value::number_type_E:
		number_(root.number, os);
		break;

	case _base_value::bool_type_E:
		bool_(root.boolean, os);
		break;

	case _base_value::array_type_E:
		array_(root, os, level);
		break;

	case _base_value::object_type_E:
		object_(root, os, level);
		break;

	default:
		break;
	}
}

std::string cpknife::escape_newlines(const std::string &input)
{
	size_t pos = 0;
	std::string target(input);

	while ((pos = target.find("\n", pos)) != std::string::npos) {
		target.replace(pos, 1, "\\n");
		pos += 2;
	}

	return target;
}

std::string cpknife::escape_double_quotes(const std::string &input)
{
	size_t pos = 0;
	std::string target(input);

	while ((pos = target.find("\"", pos)) != std::string::npos) {
		target.replace(pos, 1, "\\\"");
		pos += 2;
	}

	return target;
}

std::string cpknife::escape_backslashes(const std::string &input)
{
	size_t pos = 0;
	std::string target(input);

	while ((pos = target.find("\\", pos)) != std::string::npos) {
		target.replace(pos, 1, "\\\\");
		pos += 2;
	}

	return target;
}

void cpknife::serialize_json::indent_(size_t level, std::ostream & os)
{
	for (size_t i = 0; i < level; ++i) {
		os << "  ";
	}
}

void cpknife::serialize_json::null_(std::ostream & os)
{
	os << "null";
}

void cpknife::serialize_json::string_(const std::string & value, std::ostream & os)
{
	std::string normalized = escape_backslashes(value);
	normalized = escape_double_quotes(normalized);
	normalized = escape_newlines(normalized);

	os << "\"" << normalized << "\"";
}

void cpknife::serialize_json::number_(double value, std::ostream & os)
{
	std::stringstream str_stream;
	str_stream << std::fixed << value;
	std::string stringValue = str_stream.str();

	// "std:fixed" output always shows the decimal place and any
	// leading zero's. Let's trim those out if there are any:
	size_t position = stringValue.find_last_not_of("0");
	if (stringValue.at(position) == '.') {
		--position;
	}
	stringValue.resize(position + 1);

	os << stringValue;
}

void cpknife::serialize_json::bool_(bool value, std::ostream & os)
{
	os << (value ? "true" : "false");
}

void cpknife::serialize_json::array_(const _base_value & value, std::ostream & os, size_t level)
{
	os << "[";
	if (!value.array().empty()) {
		os << "\n";
		size_t i = 0;

		for (array_type::const_iterator it = value.array().begin();
			it != value.array().end();
			++i, ++it) {
			if (i > 0 && i < value.array().size()) {
				os << ",\n";
			}
			indent_(level + 1, os);
			process(*it, os, level + 1);
		}

		os << "\n";
		indent_(level, os);
	}

	os << "]";
}

void cpknife::serialize_json::object_(const _base_value & value, std::ostream & os, size_t level)
{
	os << "{";

	if (!value.keys.empty()) {
		os << "\n";
		size_t i = 0;

		for (object_key_type::const_iterator it = value.keys.begin();
			it != value.keys.end();
			++i, ++it) {
			if (i > 0 && i < value.keys.size()) {
				os << ",\n";
			}

			indent_(level + 1, os);
			string_(*it, os);

			os << ": ";
			process(value.object().at(*it), os, level + 1);
		}

		os << "\n";
		indent_(level, os);
	}

	os << "}";
}
