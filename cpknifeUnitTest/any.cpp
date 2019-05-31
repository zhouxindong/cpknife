#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\cpknife\any.h"

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
		}
	};
}
