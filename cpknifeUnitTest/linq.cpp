#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\cpknife\linq.h"
#include "collectutils.h"
#include <fstream>

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace cpknife;

namespace cpknifeUnitTest
{		
	ofstream of("linq_test.txt", ios::out | ios::trunc);

	TEST_CLASS(linqTest)
	{
	public:
		
		TEST_METHOD(foreachTest)
		{
			int ar[100];
			FillInBuildArray(ar, 100);
			int index = 1;
			from(ar).foreach([&](int e) {
				Assert::AreEqual(index++, e);
			});
			Assert::AreEqual(101, index);

			array<int, 1000> ary;
			FillArray(ary);
			index = 1;
			from(ary).foreach([&](int e) {
				Assert::AreEqual(index++, e);
			});
			Assert::AreEqual(1001, index);

			vector<int> vec;
			FillVector(vec, 20);
			index = 1;
			from(vec).foreach([&](int e) {
				Assert::AreEqual(index++, e);
			});
			Assert::AreEqual(21, index);

			deque<int> deq;
			FillDeque(deq, 100);
			index = 1;
			from(deq).foreach([&](int e) {
				Assert::AreEqual(index++, e);
			});
			Assert::AreEqual(101, index);

			list<int> li;
			FillList(li, 50);
			index = 1;
			from(li).foreach([&](int e) {
				Assert::AreEqual(index++, e);
			});
			Assert::AreEqual(51, index);

			forward_list<int> foli;
			FillForwardList(foli, 50);
			index = 50;
			from(foli).foreach([&](int e) {
				Assert::AreEqual(index--, e);
				int j = e;
			});
			Assert::AreEqual(0, index);

			set<int> se;
			FillSet(se, 50);
			set<int> se_copy;
			from(se).foreach([&](int e) {
				se_copy.insert(e);
			});
			for (int i=0; i<50; i++)
			{
				Assert::IsTrue(contains(i + 1, se_copy));
			}

			map<int, string> mp;
			FillMap(mp, 500);
			map<int, string> mp_copy;
			from_map(mp).foreach([&](pair<int, string> e) {
				mp_copy.insert(e);
			});
			for (int i = 0; i < 500; i++)
			{
				Assert::IsTrue(contains(i + 1, mp_copy));
			}

			unordered_set<int> udse;
			FillUnorderedSet(udse, 50);
			unordered_set<int> udse_copy;
			from_unordered_set(udse).foreach([&](int e) {
				udse_copy.insert(e);
			});
			for (int i = 0; i < 50; i++)
			{
				Assert::IsTrue(contains(i + 1, udse_copy));
			}

			unordered_map<int, string> udmp;
			FillUnorderedMap(udmp, 500);
			unordered_map<int, string> udmp_copy;
			from(udmp).foreach([&](pair<int, string> e) {
				udmp_copy.insert(e);
			});
			for (int i = 0; i < 500; i++)
			{
				Assert::IsTrue(contains(i + 1, udmp_copy));
			}
		}

		//TEST_METHOD(whereTest) {
		//	int src[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		//	int ans[] = { 1, 3, 5, 7, 9 };

		//	auto rng = from(src).where([](int a) {return a % 2 == 1; });
		//	Assert::IsTrue(InBuiltArrayEqual<int>(ans, rng));
		//}
	};
}