#pragma once

#include <string>
#include "cpknife_exports.h"

namespace cpknife {

	typedef unsigned char byte;

	class CPKNIFE_API binary
	{
	public:
		static binary* create_read(std::string stream);
		static binary* create_write();

		binary();
		virtual ~binary();

		bool init_read(std::string stream);
		bool init_write();

		const void set_endian(const bool isBigEndian);
		const bool is_big_endian() const;

		const std::string get_stream() const { return _stream; }

		byte read_byte();
		bool read_bool();
		char read_char();
		short read_short();
		int read_int();
		long long read_long();
		float read_float();
		double read_double();
		std::string read_string();  //for C# ReadString
		std::string read_string8();
		std::string read_string16();
		std::string read_string32();

		void write_byte(byte b);
		void write_bool(bool b);
		void write_char(char c);
		void write_short(short s);
		void write_int(int i);
		void write_long(long long l);
		void write_float(float f);
		void write_double(double d);
		void write_string(std::string s);  //for C# WriteString
		void write_string8(std::string s);
		void write_string16(std::string s);
		void write_string32(std::string s);

		void finish();

	public:
		template<typename T>
		T read_value()
		{
			T t;
			int len = sizeof(T);
			if (len + _pos > _size)
				return NULL;
			memcpy(&t, _stream.substr(_pos, _pos + len).c_str(), len);
			_pos += len;
			return t;
		}

		template<typename T>
		void write_value(const T &value)
		{
			const int len = sizeof(value);
			char s[len];
			memcpy(s, &value, len);
			_stream += std::string(s, len);
			_head += len;
		}

	private:
		int read7BitEncodedInt();  //for C# ReadString
		void write7BitEncodedInt(int len);  //for C# WriteString

	private:
		std::string _stream;
		int _head;
		int _pos;
		int _size;
		bool _isBigEndian = true;
	};
}
