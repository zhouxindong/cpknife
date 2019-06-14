#include "stdafx.h"
#include "binary.h"

namespace cpknife {

#define HEAD_OFFSET 4; 

	binary* binary::create_read(std::string stream)
	{
		binary* bin = new binary;
		if (bin && bin->init_read(stream))
			return bin;
		return nullptr;
	}

	binary* binary::create_write()
	{
		binary* bin = new binary;
		if (bin && bin->init_write())
			return bin;
		return nullptr;
	}

	binary::binary()
	{

	}

	binary::~binary()
	{

	}

	bool binary::init_read(std::string stream)
	{
		_stream = stream;
		_pos = 0;
		_size = _stream.size();
		return true;
	}

	bool binary::init_write()
	{
		_pos = 0;
		_size = 0;
		_stream.clear();
		write_int(sizeof(int));
		_head = 0; //data length not contained in _head value
		return true;
	}

	const void binary::set_endian(const bool isBigEndian)
	{
		_isBigEndian = isBigEndian;
	}

	const bool binary::is_big_endian() const
	{
		return _isBigEndian;
	}

	byte binary::read_byte()
	{
		return read_value<byte>();
	}

	bool binary::read_bool()
	{
		return read_value<bool>();
	}

	char binary::read_char()
	{
		return read_value<char>();
	}

	short binary::read_short()
	{
		return read_value<short>();
	}

	int binary::read_int()
	{
		return read_value<int>();
	}

	long long binary::read_long()
	{
		return read_value<long long>();
	}

	float binary::read_float()
	{
		return read_value<float>();
	}

	double binary::read_double()
	{
		return read_value<double>();
	}

	std::string binary::read_string()
	{
		const int len = read7BitEncodedInt();
		if (len + _pos > _size)
			return NULL;
		std::string s = _stream.substr(_pos, len).c_str();
		_pos += len;
		return s;
	}

	std::string binary::read_string8()
	{
		const int len = this->read_char();
		if (len + _pos > _size)
			return NULL;
		std::string s = _stream.substr(_pos, len).c_str();
		_pos += len;
		return s;
	}

	std::string binary::read_string16()
	{
		const int len = this->read_short();
		if (len + _pos > _size)
			return NULL;
		std::string s = _stream.substr(_pos, len).c_str();
		_pos += len;
		return s;
	}

	std::string binary::read_string32()
	{
		const int len = this->read_int();
		if (len + _pos > _size)
			return NULL;
		std::string s = _stream.substr(_pos, len).c_str();
		_pos += len;
		return s;
	}

	int binary::read7BitEncodedInt()
	{
		byte oneByte;
		int len = 0;
		int head = 0;
		do
		{
			if (head == 0x23)
				return 0;
			oneByte = read_byte();
			len |= (oneByte & 0x7f) << head;
			head += 7;
		} while ((oneByte & 0x80) != 0);
		return len;
	}

	void binary::write_byte(byte b)
	{
		write_value<byte>(b);
	}

	void binary::write_bool(bool b)
	{
		write_value<bool>(b);
	}

	void binary::write_char(char c)
	{
		write_value<char>(c);
	}

	void binary::write_short(short sh)
	{
		write_value<short>(sh);
	}

	void binary::write_int(int i)
	{
		write_value<int>(i);
	}

	void binary::write_long(long long l)
	{
		write_value<long long>(l);
	}

	void binary::write_float(float f)
	{
		write_value<float>(f);
	}

	void binary::write_double(double d)
	{
		write_value<double>(d);
	}

	void binary::write_string(std::string s)
	{
		write7BitEncodedInt(s.size());
		_stream += s;
		_head += (s.size() + 1);
	}

	void binary::write_string8(std::string s)
	{
		write_char(s.size());
		_stream += s;
		_head += (s.size() + sizeof(char));
	}

	void binary::write_string16(std::string s)
	{
		write_short(s.size());
		_stream += s;
		_head += (s.size() + sizeof(short));
	}

	void binary::write_string32(std::string s)
	{
		write_int(s.size());
		_stream += s;
		_head += (s.size() + sizeof(int));
	}

	void binary::write7BitEncodedInt(int len)
	{
		unsigned int num = (unsigned int)len;
		while (num >= 0x80)
		{
			write_byte((byte)(num | 0x80));
			num = num >> 7;
		}
		write_byte((byte)num);
	}

	void binary::finish()
	{
		char dataLen[sizeof(short)];
		_head -= HEAD_OFFSET;
		memcpy(dataLen, &_head, sizeof(short));
		_stream[0] = dataLen[0];
		_stream[1] = dataLen[1];
	}
}
