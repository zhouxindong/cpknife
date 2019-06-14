#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\cpknife\any_prop.h"
#include <limits>
#include <basetsd.h>
#include <fstream>

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace cpknife;

namespace cpknifeUnitTest
{


	TEST_CLASS(any_propTest)
	{
	public:
		any_prop prop;

		TEST_METHOD(any_propTest1)
		{
			prop.Set<INT8>("int8", numeric_limits<INT8>::max());
			INT8* int8 = prop.Get<INT8>("int8");
			Assert::AreEqual(numeric_limits<INT8>::max(), *int8);

			prop.Set<UINT8>("uint8", numeric_limits<UINT8>::max());
			UINT8* uint8 = prop.Get<UINT8>("uint8");
			Assert::AreEqual(numeric_limits<UINT8>::max(), *uint8);

			prop.Set<INT16>("int16", numeric_limits<INT16>::max());
			INT16* int16 = prop.Get<INT16>("int16");
			Assert::AreEqual(numeric_limits<INT16>::max(), *int16);

			prop.Set<UINT16>("uint16", numeric_limits<UINT16>::max());
			UINT16* uint16 = prop.Get<UINT16>("uint16");
			Assert::AreEqual((int)numeric_limits<UINT16>::max(), (int)*uint16);

			prop.Set<INT32>("int32", numeric_limits<INT32>::max());
			INT32* int32 = prop.Get<INT32>("int32");
			Assert::AreEqual(numeric_limits<INT32>::max(), *int32);

			prop.Set<UINT32>("uint32", numeric_limits<UINT32>::max());
			UINT32* uint32 = prop.Get<UINT32>("uint32");
			Assert::AreEqual(numeric_limits<UINT32>::max(), *uint32);

			prop.Set<INT64>("int64", numeric_limits<INT64>::max());
			INT64* int64 = prop.Get<INT64>("int64");
			Assert::AreEqual((UINT64)numeric_limits<INT64>::max(), (UINT64)*int64);

			prop.Set<UINT64>("uint64", numeric_limits<UINT64>::max());
			UINT64* uint64 = prop.Get<UINT64>("uint64");
			Assert::AreEqual(numeric_limits<UINT64>::max(), *uint64);

			prop.Set<float>("float_", numeric_limits<float>::max());
			float* float_ = prop.Get<float>("float_");
			Assert::AreEqual(numeric_limits<float>::max(), *float_);

			prop.Set<double>("double_", numeric_limits<double>::max());
			double* double_ = prop.Get<double>("double_");
			Assert::AreEqual(numeric_limits<double>::max(), *double_);

			prop.Set<bool>("bool_", true);
			bool* bool_ = prop.Get<bool>("bool_");
			Assert::IsTrue(*bool_);

			prop.Set<bool>("bool_2", false);
			bool* bool_2 = prop.Get<bool>("bool_2");
			Assert::IsFalse(*bool_2);

			prop.Set<string>("name", "abcddfdasdfasdf");
			string* str = prop.Get<string>("name");
			Assert::AreEqual(*str, string("abcddfdasdfasdf"));

			vector<int> sal{ 1,3,5,7,9 };
			prop.Set("sal", sal);

			vector<int>* sal_2 = prop.Get<vector<int>>("sal");
			Assert::AreEqual((size_t)5, (*sal_2).size());
			Assert::AreEqual(1, (*sal_2)[0]);
			Assert::AreEqual(3, (*sal_2)[1]);
			Assert::AreEqual(5, (*sal_2)[2]);
			Assert::AreEqual(7, (*sal_2)[3]);
			Assert::AreEqual(9, (*sal_2)[4]);

			//person my;
			//my.name = "ooold mmmman";
			//my.age = 10000001;
			//prop.Set("my", my);
			//person* my_ = prop.Get<person>("my");
			//Assert::AreEqual(my.name, my_->name);
			//Assert::AreEqual(my.age, my_->age);


			auto a1 = typeid(int[]).name();
			auto a2 = typeid(array<int, 10>).name();
			auto a3 = typeid(vector<int>).name();

			auto properties = prop.PropertiesInfo();

			ofstream of("any_prop.out", ios::out | ios::trunc);
			of << properties;
			of.close();
		}
	};
}