#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\cpknife\binary.h"

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace cpknife;

namespace cpknifeUnitTest
{
	//TEST_CLASS(binaryTest)
	//{
	//public:

	//	TEST_METHOD(binaryTest1)
	//	{
	//		binary wb;
	//		wb.init_write();
	//		wb.write_char('c');
	//		wb.write_int(12345);
	//		wb.write_long(23423423423);
	//		wb.write_double(1.05);
	//		wb.write_string("first string");
	//		wb.write_string("second string");
	//		wb.finish();

	//		auto _stream = wb.get_stream();

	//		binary rb;
	//		rb.init_read(wb.get_stream());
	//		int length = rb.read_int();
	//		char c = rb.read_char();
	//		int i = rb.read_int();
	//		long long l = rb.read_long();
	//		double d = rb.read_double();
	//		string s1 = rb.read_string();
	//		string s2 = rb.read_string();
	//	}
	//};
}
