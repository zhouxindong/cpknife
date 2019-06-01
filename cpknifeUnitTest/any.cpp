#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\cpknife\any.h"
#include <basetsd.h>
#include <limits>

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace cpknifeUnitTest
{
	struct obj {
		obj(char const* s) : str(s) {}
		char const* str;
	};

	TEST_CLASS(anytest)
	{
	public:

		TEST_METHOD(basetypetest)
		{
			cpknife::any a("test");
			Assert::IsTrue(typeid(char const*) == a.type());
			Assert::AreEqual(0,
				strcmp(a.as<char*>(), "test"));

			a = 3.14159;
			Assert::AreEqual(3.14159, a.as<double>());

			cpknife::any b(a);
			Assert::AreEqual(3.14159, b.as<double>());

			obj o("hello");
			a = o;
			Assert::AreEqual(0,
				strcmp(a.as<obj>().str, "hello"));

			a = new obj("world");
			Assert::AreEqual(0,
				strcmp(a.as<obj*>()->str, "world"));

			cpknife::any c(a);
			cpknife::any d(c);
			Assert::AreEqual(0,
				strcmp(d.as<obj*>()->str, "world"));

			cpknife::any t;
			t = cpknife::any(true);
			Assert::IsTrue(t.as<bool>());

			t = std::string("string class");
			Assert::AreEqual(t.as<std::string>(),
				std::string("string class"));
		}

		TEST_METHOD(valueTest)
		{
			cpknife::any a(numeric_limits<INT8>::min());
			Assert::IsTrue(typeid(INT8) == a.type());
			Assert::AreEqual(a.as<INT8>(), numeric_limits<INT8>::min());
			a = (numeric_limits<INT8>::max());
			Assert::IsTrue(typeid(INT8) == a.type());
			Assert::AreEqual(a.as<INT8>(), numeric_limits<INT8>::max());

			a = (numeric_limits<UINT8>::min());
			Assert::IsTrue(typeid(UINT8) == a.type());
			Assert::AreEqual(a.as<UINT8>(), numeric_limits<UINT8>::min());
			a = (numeric_limits<UINT8>::max());
			Assert::IsTrue(typeid(UINT8) == a.type());
			Assert::AreEqual(a.as<UINT8>(), numeric_limits<UINT8>::max());

			a = (numeric_limits<INT16>::min());
			Assert::IsTrue(typeid(INT16) == a.type());
			Assert::AreEqual(a.as<INT16>(), numeric_limits<INT16>::min());
			a = (numeric_limits<INT16>::max());
			Assert::IsTrue(typeid(INT16) == a.type());
			Assert::AreEqual(a.as<INT16>(), numeric_limits<INT16>::max());

			a = (numeric_limits<UINT16>::min());
			Assert::IsTrue(typeid(UINT16) == a.type());
			Assert::AreEqual((INT32)a.as<UINT16>(), (INT32)numeric_limits<UINT16>::min());
			a = (numeric_limits<UINT16>::max());
			Assert::IsTrue(typeid(UINT16) == a.type());
			Assert::AreEqual((INT32)a.as<UINT16>(), (INT32)numeric_limits<UINT16>::max());

			a = (numeric_limits<INT32>::min());
			Assert::IsTrue(typeid(INT32) == a.type());
			Assert::AreEqual(a.as<INT32>(), numeric_limits<INT32>::min());
			a = (numeric_limits<INT32>::max());
			Assert::IsTrue(typeid(INT32) == a.type());
			Assert::AreEqual(a.as<INT32>(), numeric_limits<INT32>::max());

			a = (numeric_limits<UINT32>::min());
			Assert::IsTrue(typeid(UINT32) == a.type());
			Assert::AreEqual(a.as<UINT32>(), numeric_limits<UINT32>::min());
			a = (numeric_limits<UINT32>::max());
			Assert::IsTrue(typeid(UINT32) == a.type());
			Assert::AreEqual(a.as<UINT32>(), numeric_limits<UINT32>::max());

			a = (numeric_limits<INT64>::min());
			Assert::IsTrue(typeid(INT64) == a.type());
			Assert::AreEqual((UINT64)a.as<INT64>(), (UINT64)numeric_limits<INT64>::min());
			a = (numeric_limits<INT64>::max());
			Assert::IsTrue(typeid(INT64) == a.type());
			Assert::AreEqual((UINT64)a.as<INT64>(), (UINT64)numeric_limits<INT64>::max());

			a = (numeric_limits<UINT64>::min());
			Assert::IsTrue(typeid(UINT64) == a.type());
			Assert::AreEqual(a.as<UINT64>(), numeric_limits<UINT64>::min());
			a = (numeric_limits<UINT64>::max());
			Assert::IsTrue(typeid(UINT64) == a.type());
			Assert::AreEqual(a.as<UINT64>(), numeric_limits<UINT64>::max());

			a = (numeric_limits<float>::min());
			Assert::IsTrue(typeid(float) == a.type());
			Assert::AreEqual(a.as<float>(), numeric_limits<float>::min());
			a = (numeric_limits<float>::max());
			Assert::IsTrue(typeid(float) == a.type());
			Assert::AreEqual(a.as<float>(), numeric_limits<float>::max());

			a = (numeric_limits<double>::min());
			Assert::IsTrue(typeid(double) == a.type());
			Assert::AreEqual(a.as<double>(), numeric_limits<double>::min());
			a = (numeric_limits<double>::max());
			Assert::IsTrue(typeid(double) == a.type());
			Assert::AreEqual(a.as<double>(), numeric_limits<double>::max());
		}
	};
}
