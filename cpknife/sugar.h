#pragma once

#define LOG(...) {\
	fprintf(stderr, "%s: Line %d:\t", __FILE__, __LINE__); \
	fprintf(stderr, __VA_ARGS__); \
	fprintf(stderr, "\n"); \
}

#pragma region auto_property

#define _DEF_PROPERTY_INIT(type, name, value) type _##name = value;
#define _DEF_PROPERTY(type, name) type _##name;

#define _GET_PROPERTY(type, name) type get_##name() const {return _##name;}
#define _GET_PROPERTY_CONSTREF(type, name) const type & get_##name() const {return _##name;}
#define _GET_PROPERTY_REF(type, name) type & get_##name() const {return _##name;}

#define _SET_PROPERTY(type, name) void set_##name(const type & value){_##name = value;}

#define _PROPERTY(type, name) \
private:\
	_DEF_PROPERTY(type, name) \
public:\
	_GET_PROPERTY_CONSTREF(type, name) \
	_SET_PROPERTY(type, name)

#define _PROPERTY_INIT(type, name, value) \
private:\
	_DEF_PROPERTY_INIT(type, name, value) \
public:\
	_GET_PROPERTY_CONSTREF(type, name) \
	_SET_PROPERTY(type, name)

#define _PROPERTY_READONLY(type, name) \
private:\
	_DEF_PROPERTY(type, name) \
public:\
	_GET_PROPERTY_CONSTREF(type, name)

#define _PROPERTY_READONLY_INIT(type, name, value) \
private:\
	_DEF_PROPERTY_INIT(type, name, value) \
public:\
	_GET_PROPERTY_CONSTREF(type, name)

#pragma endregion