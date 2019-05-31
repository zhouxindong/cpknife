#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\cpknife\sugar.h"

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace cpknifeUnitTest
{
	class A {
		_PROPERTY(int, age)
		_PROPERTY_INIT(string, name, "jionns")
	};

	TEST_CLASS(sugartest)
	{
	public:

		TEST_METHOD(LOGtest)
		{
			FILE *stream = freopen("freopen.out", "w", stderr);

			int x = 33;
			LOG("x = %d", x);
			fclose(stream);
		}

		TEST_METHOD(_PROPERTY_Test)
		{
			A a;
			
			a.set_age(35);
			Assert::AreEqual(35, a.get_age());
			Assert::AreEqual(string("jionns"), a.get_name());

			a.set_name("hahaha");
			Assert::AreEqual(string("hahaha"), a.get_name());
		}
	};
}
